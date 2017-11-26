segment .text
[bits 32]

;;
;; import / export entries between .nasm and .c parts
;;
%ifidn __OUTPUT_FORMAT__, win32
extern _EntryPoint                  ; import C entry point from main.c
EntryPoint equ _EntryPoint          ; win32 builds from Visual C decorate C names using _
%else
extern EntryPoint                   ; import C entry point from main.c
%endif

global gMultiBootHeader         ; export multiboot structures to .c
global gMultiBootStruct

;;
;; we use hardcoded address space / map for our data structures, the multiboot header and the entry point
;; the plain binary image is loaded to 0x00200000 (2MB)
;;
KERNEL_BASE_VIRTUAL_32      equ 0x40000000			    ; magic 1G VA for x86 builds
KERNEL_BASE_VIRTUAL_64      equ 0x0000000200000000	    ; magic 8G VA for x64 builds
KERNEL_BASE_PHYSICAL        equ 0x200000                ; physical address where this file will be loaded (2 MB PA)

MULTIBOOT_HEADER_BASE       equ KERNEL_BASE_PHYSICAL + 0x400 ; take into account the MZ/PE header + 0x400 allignment
                                                        ; the multiboot header begins in the .text section
MULTIBOOT_HEADER_SIZE       equ 48                      ; check out '3.1.1 The layout of Multiboot header'
MULTIBOOT_HEADER_MAGIC      equ 0x1BADB002
MULTIBOOT_HEADER_FLAGS      equ 0x00010003              ; 0x1 ==> loading of modules must pe 4K alligned, 0x2 ==> OS needs memory map
                                                        ; 0x10000 ==> OS image has valid header_addr, load_addr, ..., entry_addr

MULTIBOOT_INFO_STRUCT_BASE  equ MULTIBOOT_HEADER_BASE + MULTIBOOT_HEADER_SIZE
MULTIBOOT_INFO_STRUCT_SIZE  equ 90

MULTIBOOT_ENTRY_POINT       equ (gMultiBootEntryPoint - gMultiBootHeader) + KERNEL_BASE_PHYSICAL + 0x400

IA32_EFER                   equ 0xC0000080
CR4_PAE                     equ 0x00000020
IA23_EFER_LME               equ 0x100

PML4_BASE equ 0x1000
CR0_PE    equ 1 << 0
CR0_PG    equ 1 << 31

TOP_OF_STACK_VIRTUAL        equ KERNEL_BASE_VIRTUAL_64 + 0x10000

;;
;; KERNEL_BASE_PHYSICAL + 0x400
;;
;; *** IMPORTANT: __init.nasm MUST be the first object to be linked into the code segment ***
;;

gMultiBootHeader:                                       ; check out '3.1.1 The layout of Multiboot header'
.magic          dd MULTIBOOT_HEADER_MAGIC
.flags          dd MULTIBOOT_HEADER_FLAGS
.checksum       dd 0-(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS)
.header_addr    dd MULTIBOOT_HEADER_BASE
.load_addr      dd KERNEL_BASE_PHYSICAL
.load_end_addr  dd 0
.bss_end_addr   dd 0
.entry_addr     dd MULTIBOOT_ENTRY_POINT
.mode_type      dd 0
.width          dd 0
.height         dd 0
.depth          dd 0

gMultiBootStruct:                                       ; reserve space for the multiboot info structure (will copy here)
times MULTIBOOT_INFO_STRUCT_SIZE db 0                   ; check out '3.3 Boot information format'


;; leave 0x40 bytes for GDT stuff
times (0x100 - MULTIBOOT_HEADER_SIZE - MULTIBOOT_INFO_STRUCT_SIZE - 0x40) db 0

;;
;; N.B. here we have plenty of space (over 60KB to be used to define various data structures needed, e.g. page tables)
;;
GDT64:                           ; Global Descriptor Table (64-bit).
    .Null: equ $ - GDT64         ; The null descriptor.
    dw 0                         ; Limit (low).
    dw 0                         ; Base (low).
    db 0                         ; Base (middle)
    db 0                         ; Access.
    db 0                         ; Granularity.
    db 0                         ; Base (high).
    .Code: equ $ - GDT64         ; The code descriptor.
    dw 0                         ; Limit (low).
    dw 0                         ; Base (low).
    db 0                         ; Base (middle)
    db 10011010b                 ; Access (exec/read).
    db 00100000b                 ; Granularity.
    db 0                         ; Base (high).
    .Data: equ $ - GDT64         ; The data descriptor.
    dw 0                         ; Limit (low).
    dw 0                         ; Base (low).
    db 0                         ; Base (middle)
    db 10010010b                 ; Access (read/write).
    db 00000000b                 ; Granularity.
    db 0                         ; Base (high).
    .Pointer:                    ; The GDT-pointer.
    dw $ - GDT64 - 1             ; Limit.
    dq GDT64                     ; Base.

;;
;; KERNEL_BASE_PHYSICAL + 0x4C0
;;

;;
;; TOP-OF-STACK is KERNEL_BASE_PHYSICAL + 0x10000
;;

;;
;; N.B. multiboot starts in 32 bit PROTECTED MODE, without paging beeing enabled (FLAT); check out '3.2 Machine state' from docs
;; we explicitly allign the entry point to +64 KB (0x10000)
;;

times 0x10000 - 0x400 - $ + gMultiBootHeader db 'G'           ; allignment

;;
;; KERNEL_BASE_PHYSICAL + 0x10000
;;
[bits 32]
gMultiBootEntryPoint:
    cli

    MOV     DWORD [0x000B8000], 'O1S1'
%ifidn __OUTPUT_FORMAT__, win32
    MOV     DWORD [0x000B8004], '3121' ; 32 bit build marker
%else
    MOV     DWORD [0x000B8004], '6141' ; 64 bit build marker
%endif

    ; enable SSE instructions (CR4.OSFXSR = 1)
    mov     eax, cr4
    or      eax, 0x00000200
    mov     cr4, eax
    call __magic

    ; temporary ESP, just below code
    mov esp, stack_top

    ;lidt [IDT]

    call set_up_page_tables

    ; set address of P4 table into cr3
    mov eax, PML4T
    mov cr3, eax

    ; enable PAE
    mov eax, cr4
    or eax, CR4_PAE
    mov cr4, eax

    ; Set LM bit
    mov ecx, 0xc0000080
    rdmsr
    or eax, IA23_EFER_LME
    wrmsr

    ; enable paging
    mov eax, cr0
    or eax, CR0_PG
    mov cr0, eax

    call __magic
    lgdt [GDT64.Pointer]
    jmp GDT64.Code:longMode

    [bits 64]
    longMode:
        mov ax, 0
        mov ss, ax
        mov ds, ax
        mov es, ax
        mov fs, ax
        mov gs, ax
        mov dword [0xb8000], 0x2f4b2f4f ; print 'OK'
        hlt                           ; Halt the processor.
        call EntryPoint

	;mov DWORD [0x000B80FC], ' 0#0'
    ;mov DWORD [0x000B8100], 'S0E0'
    ;mov DWORD [0x000B8104], 'C0U0'
    ;mov DWORD [0x000B8108], 'R0E0'

    cli
    hlt

[bits 32]
set_up_page_tables:
    mov eax, PML4T
    or eax, 0b11 ; present + writable
    mov [PML4T + 511 * 8], eax

    ; map first P4 entry to P3 table
    mov eax, PDPT
    or eax, 0b11 ; present + writable
    mov [PML4T], eax

    ; map first P3 entry to P2 table
    mov eax, PDT
    or eax, 0x3        ; present + writable
    mov ecx, 512
loop_p3:
    mov [PDPT + ecx * 8], eax
    loop loop_p3
    mov [PDPT], eax

    ; map each P2 entry to a huge 2MiB page
    mov ecx, 0         ; counter variable
.map_p2_table:
    ; map ecx-th P2 entry to a huge page that starts at address 2MiB*ecx
    mov eax, 0x200000  ; 2MiB
    mul ecx            ; start address of ecx-th page
    or eax, 0x83       ; huge + present + writable
    mov [PDT + ecx * 8], eax ; map ecx-th entry

    inc ecx
    cmp ecx, 512
    jne .map_p2_table

    ret

;;--------------------------------------------------------
;; EXPORT TO C FUNCTIONS
;;--------------------------------------------------------

;%ifidn __OUTPUT_FORMAT__, win32 ; win32 builds from Visual C decorate C names using _
;global ___cli
;___cli equ __cli
;%else
;global __cli
;%endif

%macro EXPORT2C 1-*
%rep  %0
    %ifidn __OUTPUT_FORMAT__, win32 ; win32 builds from Visual C decorate C names using _
    global _%1
    _%1 equ %1
    %else
    global %1
    %endif
%rotate 1
%endrep
%endmacro

EXPORT2C __cli, __sti, __magic
__cli:
    cli
    ret

__sti:
    sti
    ret

__magic:
    xchg bx, bx
    ret

section .bss
align 4096
PML4T:
   resb 4096
PDPT:
   resb 4096
PDT:
   resb 4096
stack_bottom:
   resb 64
stack_top:

section .rodata
gdt64:
    dq 0 ; zero entry
.Code: equ $ - gdt64 ; new
    dq (1<<44) | (1<<47) | (1<<43) | (1<<53) ; code segment
.Pointer:
    dw $ - gdt64 - 1
    dq gdt64

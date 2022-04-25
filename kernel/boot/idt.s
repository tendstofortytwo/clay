.global load_idt
.type load_idt, @function
load_idt:
    lidt (idtPtr)
    sti
    ret

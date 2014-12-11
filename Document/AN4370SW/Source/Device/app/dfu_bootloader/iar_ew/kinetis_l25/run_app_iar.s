; AREA   CortexMx, CODE, READONLY      ; name

    SECTION .noinit : CODE (2)

  PUBLIC boot_app
;New_sp: r0
;New_pc: r1
boot_app:
  ;PUSH     {LR}
  msr      msp,r0  ;//set SP
  blx      r1       ;//run!
  POP      {PC}


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

  END
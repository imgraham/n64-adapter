; This file handles all communication with the N64 Controller
; It is written to make optimal use of CPU cycles in order to capture all data
#include <P18F14K50.INC>

EXTERN controller_data
EXTERN controller_data_error

cblock
    d1
    d2
    d3
    d4
	countLow
	countHigh
endc

CODE

PollController              ; Send the polling signal (0b000000011)

	; Part 1 - Read the N64 controller and store the status values
    movlw 0x20              ; 0x20 = 32 bits
    movwf d4                ; contains the number of buttons

    ;movlw controller_data              ; set up the array adress
    ;movwf FSR0L
	;movlw INDF0
	;movwf FSR0L
	LFSR 0,controller_data

    movlw 0x07              ; number of zeros that need to be sent
    movwf d2

    movlw 0x01              ; number of ones that need to be sent
    movwf d3

	movlw 0x7F
	movwf countLow
	movwf countHigh

    zero
        ;movlw 0x00
        ;movwf LATB
		bcf LATC, 2
    	bcf TRISC, 2

        movlw 0x0B
        movwf d1
        zeroloop            ; 9 instruction cycles
            decfsz d1
            goto zeroloop

        ;movlw 0x80
        ;movwf LATB
    	bsf TRISC, 2
		nop
		nop
		nop
		nop
		nop
		nop
		nop
        decfsz d2
        goto zero

    one
        ;movlw 0x00
        ;movwf LATB
		bcf LATC, 2
    	bcf TRISC, 2

        nop
        nop
        nop

        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop		
        ;movlw 0x80
        ;movwf LATB
    	bsf TRISC, 2
		nop
        movlw 0x07
        movwf d1
        oneloop             ; 8 instruction cycles
            nop
            decfsz d1
            goto oneloop
        decfsz d3
        goto one

	stopBit
		bcf LATC, 2
    	bcf TRISC, 2

        nop
        nop
        nop

        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop		
        ;movlw 0x80
        ;movwf LATB
    	bsf TRISC, 2
		nop
        movlw 0x02
        movwf d1
        sbloop             ; 8 instruction cycles
            nop
            decfsz d1
            goto sbloop

	;read the response
    bsf TRISC, 2            ; RB7 as input  (is also last instruction of poll)	

	;;;;;
	;countLow and countHigh set before polling starts

	;wait for line to go low
	waitLowLoop
		dcfsnz countLow
			goto discardData
		btfsc PORTC, 2      ; continue if low
		goto waitLowLoop

	movlw 0x7F
	movwf countLow

    readLoop                ; read all 33 bits and store them on locations 0x20 etc.

		waitHigh
			dcfsnz countLow
				goto discardData
			btfss PORTC, 2      ; continue if low
			goto waitHigh
;
		waitLow
			dcfsnz countHigh
				goto discardData
			btfsc PORTC, 2      ; continue if low
			goto waitLow

		nextBit

		  movf countLow, W
		  ;addlw 0xFF           ; eg if RAMx <= 5 ... addlw d'250'
		  subwf countHigh, W
			movlw 0xFF
		  btfsc STATUS, C

		movlw 0x00
        movwf INDF0         ; Wait & store the value of the bit in the array
        incf FSR0L          ; Go to next adress in the array

		movlw 0x7F
        movwf countLow
		movwf countHigh

        decfsz d4           ; end loop if all 33 bits are read
    goto readLoop

	;indicate no data error
	LFSR 0,controller_data_error
	movlw 0x00
    movwf INDF0         ; Wait & store the value of the bit in the array

return

discardData
	LFSR 0,controller_data_error
	movlw 0xFF
    movwf INDF0         ; Wait & store the value of the bit in the array
return
	

delay   movlw   0x5f        ; +- 1500 uS = 1.5 ms
        movwf   d1
outer   movlw   0x60
        movwf   d2
inner   nop
        nop
        decfsz  d2
        goto    inner       ; Inner loop = 5 cycles = 1uS
        decfsz  d1
        goto    outer       ; outer loop = inner + 4 cycles
return

GLOBAL PollController
end

.export _assert_true, _assert_equals

.segment "CODE"


; void fail(void)
; Throw failure using special opcode that is interpreted as a general failure on the emulator
_fail:
    anc #0

; void assert_true(unsigned char result)
_assert_true:
    cmp #0
    beq _fail ; branch if the value is zero, in that case we go to the failure state
    rts

; void assert_equals(unsigned char expected, unsigned char result)
_assert_equals:
    sta TEMP+9
    jsr popa
    cmp TEMP+9
    bne _fail ; if not equal then it is a failure
    rts

.global _start
_start:
	ldr r1, =a
	ldr r1, [r1]
	adds r0, r1, #6
	push {r0}
	swi 0

.data
	a: .word 5

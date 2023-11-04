;
; void gfx_show(GraphicComponent *gfx, uint8_t x, uint8_t y)
; TODO: Prolog can be optimized further by reordering member in GfxComponent. Probably can save a push/pop.
;
_gfx_show::
    ; Collect x & y
    ; Per calling convention, x is in a and y is pushed on the stack (before return address).
    ldhl sp, #2 ; Fetch data before return adress
    ld b, a
    ld c, (hl)
    push bc ; xy, saved for later (sp+6)

    ; Get the requested OAM
    ld h, #0
    ld l, #6 ; hl contains 6, offset to id
    add hl, de ; hl points to gfx->id
    ld a, (hl) ; a contains gfx->id
    rla ; since sizeof(oam) is 4, multiply by 2 twice
    rla ;
    ld h, #0xC0 ; shadow_OAM
    ld l, a ; hl contains offset to requested oam id
    push hl ; requested oam address, save for later (sp+4)

    ; Get the YX offset address
    ld h, #0
    ld l, #8 ; h contains 8, offset to gfx->yx_offsets
    add hl, de ; hl points to gfx->yx_offsets
    ld a, (hl+)
    ld b, (hl)
    ld c, a ; bc points to data
    push bc ; save for later (sp+2)

    ; Get count (number of oam to update)
    ld h, #0
    ld l, #7 ; hl contains 7, offset to gfx->count
    add hl, de ; hl points to gfx->count
    ld a, (hl) ; a contains gfx->count
    or a,a ; set z flag

    ; Restore
    ; yx offset pointer in hl
    pop hl
    ; oam pointer in de
    pop de
    ; x & y in bc
    pop bc

    jr z, show_loop_end

    ; Update x & y for each oam
show_loop_begin:
    dec a ; set z flag
    push af ; store new oam count

    ; it's ok to increment e only (instead of de)
    ; because oam buffer if 256-byte aligned and less than 256 bytes in size.

    ; y coord
    ld a, (hl+) ; a=*yx_offset++, (y)
    add a, c    ; a+=y
    ld (de), a  ; oam=a, (y)
    inc e       ; oam++
    
    ; x coord
    ld a, (hl+) ; a=*yx_offset++, (x)
    add a, b    ; a+=x
    ld (de), a  ; oam=a, (x)

    inc e ; next oam, sizeof(oam) is 4
    inc e
    inc e

    pop af ; load oam count
    jr nz, show_loop_begin

show_loop_end:

    ; function epilog (per calling convention)
    pop hl
    inc sp
    jp (hl)

;
; void gfx_hide(GraphicComponent *gfx)
;
_gfx_hide::
    ld h, #0
    ld l, #6 ; hl contains 6, offset to gfx->id
    ld b, h
    add hl, de ; hl points to gfx->id

    ld a, (hl+) ; a contains gfx->id, hl points to gfx->count
    ld d, (hl) ; d contains gfx->count

    rla ; since sizeof(oam) is 4, multiply by 2 twice
    rla

    ld h, #0xC0 ; shadow_OAM
    ld l, a ; de points to requested oam->y

    ld c, #4 ; bc contains 4 (sizeof(oam))

    ld a, d
    ; TODO: Do we really need to check for zero here?
    or a, a ; set z flag
    ret z

    ; At this point
    ; a contains count
    ; b contains 0
    ; c contains 4
    ; hl is dest
    ; z flag is set

hide_loop_begin:
    ld (hl), b ; oam->y=0
    add hl, bc ; next oam
    dec a ; set z flag
    jr nz, hide_loop_begin

hide_loop_end:
    ret

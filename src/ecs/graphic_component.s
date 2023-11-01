;
; void gfx_show(GraphicComponent *gfx, uint8_t x, uint8_t y)
;
_gfx_show::
    ; Collect x & y
    ; Per calling convention, x is in a and y is pushed on the stack (before return address).
    ldhl sp, #2 ; Fetch data before return adress
    ld b, a
    ld c, (hl)
    push bc ; xy, saved for later (sp+6)

    ; Get the requested OAM
    ld h, d
    ld l, e ; hl points to gfx
    ld b, #0
    ld c, #6 ; bc contains 6, offset to id
    add hl, bc ; hl points to gfx->id
    ld a, (hl) ; a contains gfx->id
    ld hl, #_shadow_OAM ; hl points to oam table
    rla ; since sizeof(oam) is 4, multiply by 2 twice
    rla ;
    ld c, a ; bc contains offset to requested oam id
    add hl, bc ; hl points to requested oam
    push hl ; requested oam address, save for later (sp+4)

    ; Get the YX offset address
    ld h, d
    ld l, e ; hl points to gfx
    ld b, #0
    ld c, #8 ; bc contains 8, offset to gfx->yx_offsets
    add hl, bc ; hl points to gfx->yx_offsets
    ld a, (hl+)
    ld b, (hl)
    ld c, a ; bc points to data
    push bc ; save for later (sp+2)

    ; Get count (number of oam to update)
    ld h, d
    ld l, e ; hl points to gfx
    ld b, #0
    ld c, #7 ; bc contains 7, offset to gfx->count
    add hl, bc ; hl points to gfx->count
    ld a, (hl) ; a contains gfx->count
    push af ; save for later (sp+0)
    
    ; Restore
    ; x & y in bc
    ldhl sp, #6
    ld a, (hl+)
    ld b, (hl)
    ld c, a
    ; oam pointer in de 
    ldhl sp, #4
    ld a, (hl+)
    ld d, (hl)
    ld e, a
    ; yx offset pointer in hl
    ldhl sp, #2
    ld a, (hl+)
    ld h, (hl)
    ld l, a

    ; Update x & y for each oam
show_loop_begin:
    pop af ; load oam count
    or a,a
    jr z, show_loop_end ; loop until oam count reaches zero
    dec a
    push af ; store new oam count
    
    ; y coord
    ld a, (hl+) ; a=*yx_offset++, (y)
    add a, c    ; a+=y
    ld (de), a  ; oam=a, (y)
    inc de      ; oam++
    
    ; x coord
    ld a, (hl+) ; a=*yx_offset++, (x)
    add a, b    ; a+=x
    ld (de), a  ; oam=a, (x)

    inc de ; next oam, sizeof(oam) is 4
    inc de
    inc de

    jp show_loop_begin

show_loop_end:
    ; clean
    pop hl
    pop hl
    pop hl
    ; function epilog (per calling convention)
    pop hl
    inc sp
    jp (hl)

;
; void gfx_hide(GraphicComponent *gfx)
;
_gfx_hide::
    ld h, d
    ld l, e ; hl points to gfx
    ld b, #0
    ld c, #6 ; bc contains 6, offset to gfx->id
    add hl, bc ; hl points to gfx->id
    ld a, (hl+) ; a contains gfx->id, hl points to gfx->count

    ld d, (hl) ; d contains gfx->count
    ld hl, #_shadow_OAM ; hl points to oam table
    rla ; since sizeof(oam) is 4, multiply by 2 twice
    rla
    ld c, a ; bc contains offset to requested oam id
    add hl, bc ; hl points to requested oam->y

    ld c, #4 ; bc contains sizeof oam
    xor a, a ; a contains zero

hide_loop_begin:
    cp a, d
    jp z, hide_loop_end ; loop until d reaches zero
    ld (hl), a ; oam->y=0
    add hl, bc ; next oam
    dec d ; d--
    jp hide_loop_begin ; end loop

hide_loop_end:
    ret

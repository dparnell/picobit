#lang racket
;;> (define frame (list 1 0 0 2 1 0 1 0 1))
;;> (crc16_calc frame (length frame))
;;17067

#|
Auhtor: Cecília Carneiro e Silva

Adaptation of:
 * FreeModbus Libary: A portable Modbus implementation for Modbus ASCII/RTU.
 * Copyright (c) 2006 Christian Walter <wolti@sil.at>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * File: $Id: mbcrc.c,v 1.7 2007/02/18 23:50:27 wolti Exp $
|#


(provide crc16_calc)

(define crc_hi_table 
  (vector
   #x00 #xC1 #x81 #x40 #x01 #xC0 #x80 #x41 #x01 #xC0 #x80 #x41
   #x00 #xC1 #x81 #x40 #x01 #xC0 #x80 #x41 #x00 #xC1 #x81 #x40
   #x00 #xC1 #x81 #x40 #x01 #xC0 #x80 #x41 #x01 #xC0 #x80 #x41
   #x00 #xC1 #x81 #x40 #x00 #xC1 #x81 #x40 #x01 #xC0 #x80 #x41
   #x00 #xC1 #x81 #x40 #x01 #xC0 #x80 #x41 #x01 #xC0 #x80 #x41
   #x00 #xC1 #x81 #x40 #x01 #xC0 #x80 #x41 #x00 #xC1 #x81 #x40
   #x00 #xC1 #x81 #x40 #x01 #xC0 #x80 #x41 #x00 #xC1 #x81 #x40
   #x01 #xC0 #x80 #x41 #x01 #xC0 #x80 #x41 #x00 #xC1 #x81 #x40
   #x00 #xC1 #x81 #x40 #x01 #xC0 #x80 #x41 #x01 #xC0 #x80 #x41
   #x00 #xC1 #x81 #x40 #x01 #xC0 #x80 #x41 #x00 #xC1 #x81 #x40
   #x00 #xC1 #x81 #x40 #x01 #xC0 #x80 #x41 #x01 #xC0 #x80 #x41
   #x00 #xC1 #x81 #x40 #x00 #xC1 #x81 #x40 #x01 #xC0 #x80 #x41 
   #x00 #xC1 #x81 #x40 #x01 #xC0 #x80 #x41 #x01 #xC0 #x80 #x41
   #x00 #xC1 #x81 #x40 #x00 #xC1 #x81 #x40 #x01 #xC0 #x80 #x41 
   #x01 #xC0 #x80 #x41 #x00 #xC1 #x81 #x40 #x01 #xC0 #x80 #x41
   #x00 #xC1 #x81 #x40 #x00 #xC1 #x81 #x40 #x01 #xC0 #x80 #x41
   #x00 #xC1 #x81 #x40 #x01 #xC0 #x80 #x41 #x01 #xC0 #x80 #x41 
   #x00 #xC1 #x81 #x40 #x01 #xC0 #x80 #x41 #x00 #xC1 #x81 #x40
   #x00 #xC1 #x81 #x40 #x01 #xC0 #x80 #x41 #x01 #xC0 #x80 #x41
   #x00 #xC1 #x81 #x40 #x00 #xC1 #x81 #x40 #x01 #xC0 #x80 #x41
   #x00 #xC1 #x81 #x40 #x01 #xC0 #x80 #x41 #x01 #xC0 #x80 #x41
   #x00 #xC1 #x81 #x40))

(define crc_low_table
  (vector
   #x00 #xC0 #xC1 #x01 #xC3 #x03 #x02 #xC2 #xC6 #x06 #x07 #xC7
   #x05 #xC5 #xC4 #x04 #xCC #x0C #x0D #xCD #x0F #xCF #xCE #x0E
   #x0A #xCA #xCB #x0B #xC9 #x09 #x08 #xC8 #xD8 #x18 #x19 #xD9
   #x1B #xDB #xDA #x1A #x1E #xDE #xDF #x1F #xDD #x1D #x1C #xDC
   #x14 #xD4 #xD5 #x15 #xD7 #x17 #x16 #xD6 #xD2 #x12 #x13 #xD3
   #x11 #xD1 #xD0 #x10 #xF0 #x30 #x31 #xF1 #x33 #xF3 #xF2 #x32
   #x36 #xF6 #xF7 #x37 #xF5 #x35 #x34 #xF4 #x3C #xFC #xFD #x3D
   #xFF #x3F #x3E #xFE #xFA #x3A #x3B #xFB #x39 #xF9 #xF8 #x38 
   #x28 #xE8 #xE9 #x29 #xEB #x2B #x2A #xEA #xEE #x2E #x2F #xEF
   #x2D #xED #xEC #x2C #xE4 #x24 #x25 #xE5 #x27 #xE7 #xE6 #x26
   #x22 #xE2 #xE3 #x23 #xE1 #x21 #x20 #xE0 #xA0 #x60 #x61 #xA1
   #x63 #xA3 #xA2 #x62 #x66 #xA6 #xA7 #x67 #xA5 #x65 #x64 #xA4
   #x6C #xAC #xAD #x6D #xAF #x6F #x6E #xAE #xAA #x6A #x6B #xAB 
   #x69 #xA9 #xA8 #x68 #x78 #xB8 #xB9 #x79 #xBB #x7B #x7A #xBA
   #xBE #x7E #x7F #xBF #x7D #xBD #xBC #x7C #xB4 #x74 #x75 #xB5
   #x77 #xB7 #xB6 #x76 #x72 #xB2 #xB3 #x73 #xB1 #x71 #x70 #xB0
   #x50 #x90 #x91 #x51 #x93 #x53 #x52 #x92 #x96 #x56 #x57 #x97
   #x55 #x95 #x94 #x54 #x9C #x5C #x5D #x9D #x5F #x9F #x9E #x5E
   #x5A #x9A #x9B #x5B #x99 #x59 #x58 #x98 #x88 #x48 #x49 #x89
   #x4B #x8B #x8A #x4A #x4E #x8E #x8F #x4F #x8D #x4D #x4C #x8C
   #x44 #x84 #x85 #x45 #x87 #x47 #x46 #x86 #x82 #x42 #x43 #x83
   #x41 #x81 #x80 #x40 ))

(define (crc16_calc r_frame size)
  (let loop ( (iIndex  0)
              (crc_low #xff)
              (crc_hi  #xff)
              (len size)
              (frame r_frame) )
    (if (= len 0)
        (bitwise-ior (arithmetic-shift crc_hi 8) crc_low)
        (let* ( (new_frame  frame)
                (new_iIndex (bitwise-xor crc_low (car new_frame))) )

          (loop new_iIndex
                (bitwise-xor crc_hi (vector-ref crc_hi_table new_iIndex))
                (vector-ref crc_low_table new_iIndex)
                (- len 1)
                (cdr new_frame)  ) ) )) )
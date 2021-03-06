/*
 * Copyright (C) 2008-2013 NEC Corporation
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License, version 2, as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */


#include <stdint.h>
#include "trema.h"
#include "ofdp.h"
#include "oxm.h"


static uint32_t eth_dst_field( const bool attr, const enum oxm_ofb_match_fields oxm_type );
static uint16_t eth_dst_length( const match *match );
static uint16_t pack_eth_dst( oxm_match_header *hdr, const match *match );


static struct oxm oxm_eth_dst = {
  OFPXMT_OFB_ETH_DST,
  OFP_ETH_ALEN + sizeof( oxm_match_header ),
  eth_dst_field,
  eth_dst_length,
  pack_eth_dst
};


void 
init_oxm_eth_dst( void ) {
  register_oxm( &oxm_eth_dst );
}


static uint32_t
eth_dst_field( const bool attr, enum oxm_ofb_match_fields oxm_type ) {
  uint32_t field = 0;

  if ( attr && oxm_type == oxm_eth_dst.type ) {
    field = OXM_OF_ETH_DST;
  }
  return field;
}


static uint16_t
eth_dst_length( const match *match ) {
  uint16_t length = 0;
  
  if ( match->eth_dst[ 0 ].valid ) {
    length = oxm_eth_dst.length;
    if ( match->eth_dst[ 0 ].mask != UINT8_MAX ) {
      length = ( uint16_t ) ( length * 2 );
    }
  }
  return length;
}


static uint16_t
pack_eth_dst( oxm_match_header *hdr, const match *match ) {
  if ( match->eth_dst[ 0 ].valid ) {
    *hdr = OXM_OF_ETH_DST;
    uint8_t *value = ( uint8_t * ) ( ( char * ) hdr + sizeof( oxm_match_header ) );
    memcpy( value, &match->eth_dst[ 0 ].value, OFP_ETH_ALEN );
    return oxm_eth_dst.length;
  }
  return 0;
}


/*
 * Local variables:
 * c-basic-offset: 2
 * indent-tabs-mode: nil
 * End:
 */

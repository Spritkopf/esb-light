#ifndef ESB_PROTOCOL_H_
#define ESB_PROTOCOL_H_

#include <stdint.h>
#include "esb.h"


#define ESB_PROT_ERR_OK          0x00
#define ESB_PROT_ERR_SIZE        0x01  /* invalid payload length */
#define ESB_PROT_ERR_CMD         0x02  /* unknown command */

/* initialize Enhanced Shockburst (ESB) communication protocol

 * ESB_ERR_OK       - OK
 * ESB_ERR_HAL      - ESB HAL Error
 * ESB_ERR_PARAM    - Parameter Error (NULL Pointer)
*/
int8_t esb_protocol_init(void);

/* \brief Process incoming messages 
 * \retval ESB_ERR_OK       OK
 */
int8_t esb_protocol_process(void);

#endif  // ESB_PROTOCOL_H_
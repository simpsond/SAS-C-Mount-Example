#include <clib/alib_protos.h>
#include <dos/dosextens.h>
#include <proto/exec.h>
#include <proto/dos.h>
#include "mount.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
	struct DeviceList *dl;
	struct MsgPort *port, *replyport;
	struct Message *msg;
	struct DosPacket *pkt;

	if(argc<2) {
		fprintf(stderr, "USAGE: %s <volumename>\n", argv[0] ? argv[0] : "");
		return(20);
	}
	if(port = CreatePort(0L, 0L)) {
		if(dl = Mount(argv[1], port)) {
			printf("Waiting for packets.  Hit CTRL-C to exit.\n");
			while(1) {
				while(msg = GetMsg(port)) {
					int success = 0;
					pkt = (struct DosPacket *)msg->mn_Node.ln_Name;
					printf("DOS packet type %d received\n", pkt->dp_Type);
	
					if(pkt->dp_Type == 8 ) {
						printf("here!\n");
					}				
	
					switch(pkt->dp_Type) {
						case ACTION_IS_FILESYSTEM:
							printf("ACTION_IS_FILESYSTEM has been called\n");
							success = 1;
							pkt->dp_Res1 = DOSTRUE;
							break;
						// case ACTION_LOCATE_OBJECT:
						case 8:
							printf("ACTION_LOCATE_OBJECT has been called\n");
							// success = 1;
							// pkt->dp_Res1 = DOSTRUE;
							break;
					}

					
					if(success == 0 ) {
						printf("Action is not handled, will send DOSFALSE\n");
						/* Flush the packet back to its origin with an error */
						pkt->dp_Res1 = DOSFALSE;
						pkt->dp_Res2 = ERROR_ACTION_NOT_KNOWN;
					}
					replyport = pkt->dp_Port;
					pkt->dp_Port = port;
					PutMsg(replyport, pkt->dp_Link);
				}
				if(Wait( (1<<port->mp_SigBit) | SIGBREAKF_CTRL_C) & SIGBREAKF_CTRL_C) {
				       break;
				}
			}
			DisMount(dl);
		} else {
			fprintf(stderr, "Can't mount volume \"%s\"\n", argv[1]);
		}
		DeletePort(port);
	} else {
		fprintf(stderr, "Can't create port\n");
	}
	return 0;
}

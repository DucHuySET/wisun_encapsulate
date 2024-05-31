/*----import section----*/
#include <stdio.h>
#include <string.h>

#include "lib/utils.h"
#include "lib/spinel/spinel.h"
#include "lib/spinel/wpan_api.h"

/*----define section*/
#define MAX_INPUT 100
#define MALLOC_TRIM 10
#define MAX_FRAME_SIZE 1024

char token = 0;

/*----main----*/
void main()
{
    while (1)
    {
        /*---get input section---*/
        char inputStr[MAX_FRAME_SIZE];
        fgets(inputStr, sizeof(inputStr), stdin);
        size_t len = strlen(inputStr);
        if (len > 0 && inputStr[len - 1] == '\n')
        {
            inputStr[len - 1] = '\0'; // Remove the newline character
            len--;
        }

        /*---trim setion---*/
        int wordCount = 0;
        char **cmd_array = trimSentence(inputStr, &wordCount);

        int i = 0;
        for (i; i < wordCount; i++)
        {
            printf("%s\n", cmd_array[i]);
        }
    }
}

do_coap()
{
    /*
    coap <ipv6 address> <coap request code (get|put|post)> <coap request type (con|non)> <uri_path>
             [--led_state <led_target (r|g)> <led_state (0|1)>]
             [--test_option <option_number> [<option_payload>]]

            Send a coap request. The generated coap request is designed to target the ns_coap_node project,
            allowing the NCP device to get/set the state of LaunchPad LEDs via the target's "led" CoAP resource.

            Parameters:
                ipv6 address:      Destination address for coap request. Multicast addresses are not supported.
                coap request code: Specify get, put, or post as the CoAP request code
                coap request type: Specify con (confirmable) or non (non-confirmable) as the CoAP request type.
                uri_path:          Specify the path of the URI resource. Specify led to target the ns_coap_node
                                   LED resource.
                --led_state:       Specify --led_state followed by the target LED (r or RLED or g for GLED) and
                                   state to set the LED (0 for off, 1 for on). Only valid for put or post requests.
                --test_option:     Optional argument to add an additional option to the request. Specify
                                   --test_option followed by an option number and option payload. See RFC7252 for
                                   details on CoAP options.

            Examples:
                Get request (confirmable):
                    > coap fdde:ad00:beef:0:558:f56b:d688:799 get con led

                Get request (nonconfirmable):
                    > coap fdde:ad00:beef:0:558:f56b:d688:799 get non led

                Post request (set RLED to on state)
                    > coap fdde:ad00:beef:0:558:f56b:d688:799 post con led --led_state r 1

                Get request with test option:
                    > coap fdde:ad00:beef:0:558:f56b:d688:799 get con led --test_option 3 hostname
    */

    int router_state = 0;
    // router_state = prop_get_value(SPINEL_PROP_NET_STATE);
    if (router_state < 5)
    {
        print("Error: Device must be in join state 5 (Successfully joined and operational) to process coap commands");
        return;
    }
}
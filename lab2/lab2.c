#include <lcom/lcf.h>
#include <lcom/lab2.h>

#include <stdbool.h>
#include <stdint.h>

#include "lcom/timer.h"
#include "../proj/src/drivers/i8042.h"
#include "../proj/src/drivers/utils.h"
#include "../proj/src/drivers/timer.h"


extern size_t counter;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab2/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab2/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(timer_test_read_config)(uint8_t timer, enum timer_status_field field) {

    uint8_t st;

    if (timer_get_conf(timer, &st)) return 1;
    
    if (timer_display_conf(timer, st, field)) return 1;
    
    return 0;
}


int(timer_test_time_base)(uint8_t timer, uint32_t freq) {
  
  if (timer_set_frequency(timer, freq)) return 1;

  return 0;
}

int(timer_test_int)(uint8_t time) {


  uint8_t bit_no;

  if (timer_subscribe(&bit_no)) return 1;

  int ipc_status, r;
  message msg;

  while(time > 0) { 
    /* Get a request message. */
    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
        printf("driver_receive failed with %d", r);
        continue;
    }
    if (is_ipc_notify(ipc_status)) { /* received notification */
        switch (_ENDPOINT_P(msg.m_source)) {
          case HARDWARE: /* hardware interrupt notification */		
                
                if (msg.m_notify.interrupts & bit_no) { /* subscribed interrupt */
                    timer_int_handler();
                    if (counter % 60 == 0) {
                      timer_print_elapsed_time();
                      time--;
                    }
                }
                break;
          default:
                break; /* no other notifications expected do nothing */	
        }
    } else { /* received a standard message, not a notification */
        /* no standard messages expected do nothing */
    }
  }

  if (timer_unsubscribe()) return 1;

  return 0;
}

/* status_display.h */

#ifndef status_display_h_
#define status_display_h_

#ifdef __cplusplus
extern "C" {
#endif

extern void status_display_start();
extern void status_display_end();
extern void set_status(const char* title, const char* value);
extern void append_status_line(const char* title);
extern void append_nonstatus_line(const char* title);

#ifdef __cplusplus
}
#endif

#endif 	// !status_display_h_


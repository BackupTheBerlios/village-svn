#include "textrender.h"

SDL_Surface* text_render(const char *text, library_t *chars) {

  /* obliczam wielkosci znakow i tekstu */

  int char_w = chars->items[0].image->w;
  int char_h = chars->items[0].image->h;

  int width = 0, height = char_h, cur_width = 0;
  int i;

  for (i=0;;i++) {
    if (text[i] == '\0') {
      break;
    } else if (text[i] == '\n') {
      cur_width = 0;
      height += char_h;
    } else if (text[i] < 0) {
      cur_width += char_w;      
      if (cur_width > width) width = cur_width;
      i++;
    } else {
      cur_width += char_w;
      if (cur_width > width) width = cur_width;
    }
  }
  
  /* tworze nowa powierzchnie */

  SDL_Surface *canvas = SDL_CreateRGBSurface(SDL_HWSURFACE, width, height, 24, 0, 0, 0, 0);
  SDL_FillRect(canvas, NULL, SDL_MapRGB(canvas->format, 255, 0, 255));
  SDL_SetColorKey(canvas, SDL_SRCCOLORKEY, SDL_MapRGB(canvas->format, 255, 0, 255));

  /* pisze co trzeba */

  SDL_Rect dst;
  dst.x = 0;
  dst.y = 0;
  for (i=0;i<strlen(text);i++) {
    if (text[i] == '\n') {
      dst.y += char_h;
      dst.x = 0;
    } else if (text[i] == ' ') {
      dst.x += char_w;
    } else {
      char ch[3];
      if (text[i] < 0) {
	sprintf(ch,"%c%c", text[i], text[i+1]);
	i++;
      } else sprintf(ch,"%c", text[i]);

      SDL_Surface *character = library_find(chars, ch);
      if (character != NULL)
	SDL_BlitSurface(character, NULL, canvas, &dst);
      dst.x += char_w;
    }
  }
  return canvas;
  
}

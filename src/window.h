#pragma once

#ifndef WINDOW_H
#define WINDOW_H

#include <SDL.h>
#include <mytypes.h>

#include <string>
#include <tuple>
#undef main

struct Window {
  struct Iterator {
    Iterator operator++() {
      if (++(coords[0]) >= maxX) {
        (coords[0]) = 0;
        ++(coords[1]);
        (coords[2]) = maxX * (coords[1]);
      } else {
        ++(coords[2]);
      }
      return *this;
    }
    Iterator *operator*() { return this; }
    friend bool operator==(const Iterator &a, const Iterator &b) {
      return a.coords[2] == b.coords[2];
    }
    friend bool operator!=(const Iterator &a, const Iterator &b) {
      return a.coords[2] != b.coords[2];
    }

    int x() { return coords[0]; }
    int y() { return coords[1]; }
    int i() { return coords[2]; }

   private:
    Iterator(int x, int y, int i, int maxX) : maxX(maxX) {
      coords[0] = x;
      coords[1] = y;
      coords[2] = i;
    }
    int coords[3] = {0};
    int maxX;

    friend Window;
  };

  Window(int numRows, int numCols, int pixelWidth)
      : numRows(numRows),
        numCols(numCols),
        pixelWidth(pixelWidth),
        numPixel(numRows * numCols) {
    SDL_CreateWindowAndRenderer(numRows * pixelWidth, numCols * pixelWidth, 0,
                                &window, &renderer);
  }
  Iterator begin() { return Iterator(0, 0, 0, numCols); }
  Iterator end() {
    return Iterator(numCols, numRows, numRows * numCols - 1, numCols);
  }

  void setTitle(std::string title) {
    SDL_SetWindowTitle(window, title.c_str());
  }
  void clear() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
  }
  void plotWeights(double *weights);
  void plotVector2(MyTypes::arrayd<2> *vectors);
  int screenToLocal(int screen) { return screen / pixelWidth; }

  SDL_Window *getWindow() { return window; }
  SDL_Renderer *getRenderer() { return renderer; }

  const int numPixel, pixelWidth;
  const int numRows, numCols;
  size_t IX(size_t x, size_t y) { return x + numRows * y; }
  bool saveScreenshotBMP(std::string filepath);

 private:
  SDL_Window *window;
  SDL_Renderer *renderer;
};

#endif
#include "ImageUtils.h"

#include <stdio.h>
#include "base64.h"

namespace ImageUtils {
  void imageLoadedCallback(sp_image* image, void* rawPointerToBase64Image) {
    char** pointerToBase64Image = (char**)rawPointerToBase64Image;
    *pointerToBase64Image = convertImageToBase64(image);
    sp_image_remove_load_callback(image, &imageLoadedCallback, rawPointerToBase64Image);
    sp_image_release(image);
  }

  char* convertImageToBase64(sp_image* image) {
    size_t imageSize;
    int base64Size;
    const void* imageData = sp_image_data(image, &imageSize);
    return base64(imageData, (int)imageSize, &base64Size);
  }

}
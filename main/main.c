#include <stdio.h>
#include <string.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include "esp_err.h"
#include "esp_log.h"
#include "esp_vfs_fat.h"
#include "gaga.h"


static const char *TAG = "example";



void app_main(void)
{
    app_main2();

    ESP_LOGI(TAG, "Example end");
}

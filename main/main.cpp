#include <stdio.h>
#include <string.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include "esp_err.h"
#include "esp_log.h"
#include "esp_vfs_fat.h"
#include "lib/tinyxml2/tinyxml2.h"

using namespace tinyxml2;

static const char *TAG = "example";

// Handle of the wear levelling library instance
static wl_handle_t s_wl_handle = WL_INVALID_HANDLE;

// Mount path for the partition
const char *base_path = "/spiflash";

extern "C" void app_main(void)
{
    // Do example setup
    ESP_LOGI(TAG, "Setting up...");
    esp_vfs_fat_mount_config_t mount_config;
    mount_config.max_files = 4;
    mount_config.format_if_mount_failed = true;
    mount_config.allocation_unit_size = CONFIG_WL_SECTOR_SIZE;

    esp_err_t err = esp_vfs_fat_spiflash_mount(base_path, "storage", &mount_config, &s_wl_handle);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to mount FATFS (%s)", esp_err_to_name(err));
        return;
    }

    // The sample XML is embedded binary data. Create a file first containing the embedded
    // so it can be accessed.
    ESP_LOGI(TAG, "Copying sample XML to filesystem...");

    extern const char data_start[] asm("_binary_sample_xml_start");
    extern const char data_end[]   asm("_binary_sample_xml_end");
    FILE *f = fopen("/spiflash/sample.xml", "wb");

    if (f == NULL) {
        ESP_LOGE(TAG, "Failed to open file for writing");
        return;
    }
    fwrite(data_start, sizeof(char), data_end - data_start + 1, f);
    fclose(f);

    // Now that the file is created, load it using tinyxml2 and parse
    ESP_LOGI(TAG, "Reading XML file");

    XMLDocument data;
//    data.LoadFile("/spiflash/sample.xml");
    char ga[]="<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
              "  <xml_result>\n"
              "      <read_sentence lan=\"en\" type=\"study\" version=\"7.0.0.1020\">\n"
              "          <rec_paper>\n"
              "              <read_chapter accuracy_score=\"4.957704\" beg_pos=\"0\" content=\"house\" end_pos=\"88\" except_info=\"0\" fluency_score=\"4.231972\" integrity_score=\"5.000000\" is_rejected=\"false\" reject_type=\"0\" score_pattern=\"loose\" standard_score=\"4.540527\" total_score=\"4.698267\" word_count=\"1\">\n"
              "                  <sentence accuracy_score=\"4.707704\" beg_pos=\"0\" content=\"house\" end_pos=\"88\" fluency_score=\"3.990986\" index=\"0\" standard_score=\"4.290527\" total_score=\"4.450971\" word_count=\"1\">\n"
              "                      <word beg_pos=\"3\" content=\"house\" dp_message=\"0\" end_pos=\"60\" global_index=\"0\" index=\"0\" pitch=\"  231.08  231.08  235.75  236.25  235.38  234.63  234.22  234.08  234.46  235.28  236.22  236.86  236.87  236.37  235.43  234.35  232.97  231.14  228.64  225.85  222.88  220.51  218.81  218.03  217.91  217.93  218.02  217.48  216.44  213.56  210.50  207.54\" pitch_beg=\"9\" pitch_end=\"41\" property=\"0\" total_score=\"4.873125\">\n"
              "                          <syll beg_pos=\"3\" content=\"hh aw s\" end_pos=\"60\" serr_msg=\"0\" syll_accent=\"0\" syll_score=\"4.792765\">\n"
              "                              <phone beg_pos=\"3\" content=\"hh\" dp_message=\"0\" end_pos=\"12\" gwpp=\"-0.048234\"></phone>\n"
              "                              <phone beg_pos=\"12\" content=\"aw\" dp_message=\"0\" end_pos=\"36\" gwpp=\"-0.019774\"></phone>\n"
              "                              <phone beg_pos=\"36\" content=\"s\" dp_message=\"0\" end_pos=\"60\" gwpp=\"-0.550658\"></phone>\n"
              "                          </syll>\n"
              "                      </word>\n"
              "                  </sentence>\n"
              "              </read_chapter>\n"
              "          </rec_paper>\n"
              "      </read_sentence>\n"
              "  </xml_result>";
    data.Parse(ga, strlen(ga));

    XMLPrinter printer;
    data.Print(&printer);

    ESP_LOGI(TAG, "Read XML data:\n%s", printer.CStr());

    const char* to_data = data.FirstChildElement("xml_result")
            ->FirstChildElement("read_sentence")
            ->FirstChildElement("rec_paper")
            ->FirstChildElement("read_chapter")
            ->Attribute("total_score");
//    const char* from_data = data.FirstChildElement("note")->FirstChildElement("from")->GetText();
//    const char* heading_data = data.FirstChildElement("note")->FirstChildElement("heading")->GetText();
//    const char* body_data = data.FirstChildElement("note")->FirstChildElement("body")->GetText();

if(to_data){
    ESP_LOGI(TAG, "%s",to_data);

}

    ESP_LOGI(TAG, "Example end");
}

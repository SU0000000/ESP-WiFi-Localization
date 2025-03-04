#include "mdf_common.h"
#include "mwifi.h"
#include <string.h>
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "driver/i2c.h"

#define SCL_PIN          19               /*!< I2C master clock */
#define SDA_PIN          18               /*!< I2C master data  */
#define	Brightness	0xCF
#define X_WIDTH 	128
#define Y_WIDTH 	64

int8_t if_connect=0; 

static int g_sockfd    = -1;
static const char *TAG = "mwifi_examples";
//离线坐标数据
int offline_data[][3] ={
		{69, 72, 73},// (0,0)
		{71, 63, 73},// (1,0)  71,63,73
		{70, 59, 74},// (2,0)
		{56, 68, 71},// (0,1)
		{68, 63, 69},// (1,1)
		{74, 62, 71},// (2,1)
		{78, 68, 69},// (0,2)  78,68,69
		{70, 68, 65},// (1,2)
		{68, 64, 65},// (2,2)
};
// int offline_data[][3] ={
// 		{74, 75, 70},// (0,0)
// 		{64, 66, 74},// (0,1)
// 		{55, 68, 70},// (0,2)
// 		{65, 68, 70},// (0,3)
// 		{72, 65, 73},// (1,0)
// 		{64, 67, 78},// (1,1)
// 		{62, 61, 71},// (1,2)
// 		{66, 62, 66},// (1,3)
// 		{67, 61, 71},// (2,0)
// 		{66, 61, 70},// (2,1)
// 		{67, 61, 68},// (2,2)
// 		{66, 63, 63},// (2,3)		
//      {78, 61, 70},// (3,0)
// 		{70, 57, 70},// (3,1)
// 		{66, 59, 71},// (3,2)
// 		{69, 63, 63},// (3,3)

// };
//其字符串对应的坐标
int position[9][2]={{0,0},
                    {1,0},
                    {2,0},
                    {0,1},
                    {1,1},
                    {2,1},
                    {0,2},
                    {1,2},
                    {2,2}};
// char position[16][8]={"(0,0)","(0,1)","(0,2)","(0,3)",
//                       "(1,0)","(1,1)","(1,2)","(1,3)",
//                       "(2,0)","(2,1)","(2,2)","(2,3)",
//                       "(3,0)","(3,1)","(3,2)","(3,3)"
//                       };                      
                    
void tcp_client_write_task(void *arg)
{
    mdf_err_t ret = MDF_OK;
    char *data    = MDF_CALLOC(1, MWIFI_PAYLOAD_LEN);
    size_t size   = MWIFI_PAYLOAD_LEN;
    uint8_t src_addr[MWIFI_ADDR_LEN] = {0x0};
    mwifi_data_type_t data_type      = {0x0};
    //不同node发送过来的RSSI值记录
    int node1= 0;
    int node2= 0;
    int node3= 0;

    //node的编号：1,2,3
    char nodes[2];
    //node的值
    char value[5];
    //定位点的坐标选取
    int posi_num=0, posi_num2=0,posi_num3=0;
    float x,y=0.0;
    //MDF_LOGI("TCP client write task is running");

    while (mwifi_is_connected()) {
        if (g_sockfd == -1) {
            vTaskDelay(500 / portTICK_RATE_MS);
            continue;
        }

        size = MWIFI_PAYLOAD_LEN - 1;
        memset(data, 0, MWIFI_PAYLOAD_LEN);
        ret = mwifi_root_read(src_addr, &data_type, data, &size, portMAX_DELAY);
        MDF_ERROR_CONTINUE(ret != MDF_OK, "<%s> mwifi_root_read", mdf_err_to_name(ret));

        char *json_data = NULL;
        int real_value;
        sprintf(nodes,"%c",data[4]);
        sprintf(value,"%c%c",data[7],data[8]);
        real_value = atoi(value);
        if (!strcmp(nodes,"1"))
        {
            node1=real_value;
        }
        else if (!strcmp(nodes,"2"))
        {
            node2=real_value;
        }
        else if (!strcmp(nodes,"3"))
        {
            node3=real_value;
        }
        for (int delta=625,smal2=729,smal3=900,smal=delta,i=0;i<9;i++)//设置三个较大的数
        {
            delta=abs((offline_data[i][0]-node1)*(offline_data[i][0]-node1))+abs((offline_data[i][1]-node2)*(offline_data[i][1]-node2))+abs((offline_data[i][2]-node3)*(offline_data[i][2]-node3));
            if(delta<smal && delta<smal2 && delta<smal3)
            {   
                smal3=smal2;
                smal2=smal;
                smal=delta;
                posi_num=i;
            }
            else if (delta>smal && delta<smal2 && delta<smal3)
            {
                smal3=smal2;
                smal2=delta;
                posi_num2=i;
            }
            else if (delta>smal && delta>smal2 && delta<smal3)
            {
                smal3=delta;
                posi_num3=i;
            }
        }
        //WKNN
        x=(0.6*position[posi_num][0]+0.3*position[posi_num2][0]+0.1*position[posi_num3][0]);//3.0;
        y=(0.6*position[posi_num][1]+0.3*position[posi_num2][1]+0.1*position[posi_num3][1]);//3.0;KNN是除以3，但是WKNN不是！！
        int json_size   = asprintf(&json_data, "Result (%.2f,%.2f) %s", x, y,data);
        MDF_LOGI("TCP write, size: %d, data: %s", json_size, json_data);
        ret = write(g_sockfd, json_data, json_size);
        
        MDF_FREE(json_data);
        MDF_ERROR_CONTINUE(ret <= 0, "<%s> TCP write", strerror(errno));
    }

    MDF_LOGI("TCP client write task is exit");

    close(g_sockfd);
    MDF_FREE(data);
    vTaskDelete(NULL);
}

/**
 * @brief Create a tcp client
 */
static int socket_tcp_client_create(const char *ip, uint16_t port)
{
    MDF_PARAM_CHECK(ip);

    MDF_LOGI("Create a tcp client, ip: %s, port: %d", ip, port);

    mdf_err_t ret = ESP_OK;
    int sockfd    = -1;
    struct sockaddr_in server_addr = {
        .sin_family = AF_INET,
        .sin_port = htons(port),
        .sin_addr.s_addr = inet_addr(ip),
    };

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    MDF_ERROR_GOTO(sockfd < 0, ERR_EXIT, "socket create, sockfd: %d", sockfd);

    ret = connect(sockfd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr_in));
    MDF_ERROR_GOTO(ret < 0, ERR_EXIT, "socket connect, ret: %d, ip: %s, port: %d",
                   ret, ip, port);
    return sockfd;

ERR_EXIT:

    if (sockfd != -1) {
        close(sockfd);
    }

    return -1;
}

void tcp_client_read_task(void *arg)
{
    mdf_err_t ret = MDF_OK;
    char *data    = MDF_MALLOC(MWIFI_PAYLOAD_LEN);
    size_t size   = MWIFI_PAYLOAD_LEN;
    uint8_t dest_addr[MWIFI_ADDR_LEN] = {0x0};
    mwifi_data_type_t data_type       = {0x0};

    MDF_LOGI("TCP client read task is running");

    while (mwifi_is_connected()) {
        if (g_sockfd == -1) {
            g_sockfd = socket_tcp_client_create(CONFIG_SERVER_IP, CONFIG_SERVER_PORT);

            if (g_sockfd == -1) {
                vTaskDelay(500 / portTICK_RATE_MS);
                continue;
            }
        }

        memset(data, 0, MWIFI_PAYLOAD_LEN);
        ret = read(g_sockfd, data, size);
        MDF_LOGI("TCP read, %d, size: %d, data: %s", g_sockfd, size, data);

        if (ret <= 0) {
            MDF_LOGW("<%s> TCP read", strerror(errno));
            close(g_sockfd);
            g_sockfd = -1;
            continue;
        }

        cJSON *pJson = NULL;
        cJSON *pSub  = NULL;

        pJson = cJSON_Parse(data);
        MDF_ERROR_CONTINUE(!pJson, "cJSON_Parse, data format error");

        pSub = cJSON_GetObjectItem(pJson, "addr");

        if (!pSub) {
            MDF_LOGW("cJSON_GetObjectItem, Destination address not set");
            cJSON_Delete(pJson);
            continue;
        }

        /**
         * @brief  Convert mac from string format to binary
         */
        do {
            uint32_t mac_data[MWIFI_ADDR_LEN] = {0};
            sscanf(pSub->valuestring, MACSTR,
                   mac_data, mac_data + 1, mac_data + 2,
                   mac_data + 3, mac_data + 4, mac_data + 5);

            for (int i = 0; i < MWIFI_ADDR_LEN; i++) {
                dest_addr[i] = mac_data[i];
            }
        } while (0);

        pSub = cJSON_GetObjectItem(pJson, "data");

        if (!pSub) {
            MDF_LOGW("cJSON_GetObjectItem, Failed to get data");
            cJSON_Delete(pJson);
            continue;
        }

        char *json_data = cJSON_PrintUnformatted(pSub);

        ret = mwifi_root_write(dest_addr, 1, &data_type, json_data, strlen(json_data), true);
        MDF_ERROR_CONTINUE(ret != MDF_OK, "<%s> mwifi_root_write", mdf_err_to_name(ret));

        MDF_FREE(json_data);
        cJSON_Delete(pJson);
    }

    MDF_LOGI("TCP client read task is exit");

    close(g_sockfd);
    g_sockfd = -1;
    MDF_FREE(data);
    vTaskDelete(NULL);
}

static void node_read_task(void *arg)
{
    mdf_err_t ret = MDF_OK;
    cJSON *pJson = NULL;
    cJSON *pSub  = NULL;
    char *data    = MDF_MALLOC(MWIFI_PAYLOAD_LEN);
    size_t size   = MWIFI_PAYLOAD_LEN;
    mwifi_data_type_t data_type      = {0x0};
    uint8_t src_addr[MWIFI_ADDR_LEN] = {0x0};

    MDF_LOGI("Note read task is running");

    for (;;) {
        if (!mwifi_is_connected()) {
            vTaskDelay(500 / portTICK_RATE_MS);
            continue;
        }

        size = MWIFI_PAYLOAD_LEN;
        memset(data, 0, MWIFI_PAYLOAD_LEN);
        ret = mwifi_read(src_addr, &data_type, data, &size, portMAX_DELAY);
        MDF_ERROR_CONTINUE(ret != MDF_OK, "<%s> mwifi_read", mdf_err_to_name(ret));
        MDF_LOGD("Node receive: " MACSTR ", size: %d, data: %s", MAC2STR(src_addr), size, data);

        pJson = cJSON_Parse(data);
        MDF_ERROR_CONTINUE(!pJson, "cJSON_Parse, data format error, data: %s", data);

        pSub = cJSON_GetObjectItem(pJson, "status");

        if (!pSub) {
            MDF_LOGW("cJSON_GetObjectItem, Destination address not set");
            cJSON_Delete(pJson);
            continue;
        }

        gpio_set_level(CONFIG_LED_GPIO_NUM, pSub->valueint);

        cJSON_Delete(pJson);
    }

    MDF_LOGW("Note read task is exit");

    MDF_FREE(data);
    vTaskDelete(NULL);
}

static void node_write_task(void *arg)
{
    //mdf_err_t ret = MDF_OK;
    //int count     = 0;
    //size_t size   = 0;
    //char *data    = NULL;
    //mwifi_data_type_t data_type      = {0x0};
    
    //测试RSSI
    //mesh_assoc_t mesh_assoc = {0x0};
    //ret = esp_wifi_vnd_mesh_get(&mesh_assoc);
    //int8_t Rssi=0;

    MDF_LOGI("NODE task is running");

    // for (;;) {
    //     if (!mwifi_is_connected()) {
    //         vTaskDelay(500 / portTICK_RATE_MS);
    //         continue;
    //     }
    //     esp_wifi_vnd_mesh_get(&mesh_assoc);
    //     Rssi = mesh_assoc.rssi;
    //     //子节点发送
    //     //size = asprintf(&data, "<Node3> seq:%d, layer:%d, RSSI:%d",
    //     //                count++, esp_mesh_get_layer(), Rssi);
    //     size = asprintf(&data, "Node0:%d",Rssi);


    //     MDF_LOGD("Node send, size: %d, data: %s", size, data);
    //     ret = mwifi_write(NULL, &data_type, data, size, true);
    //     MDF_FREE(data);
    //     MDF_ERROR_CONTINUE(ret != MDF_OK, "<%s> mwifi_write", mdf_err_to_name(ret));

    //     vTaskDelay(5000 / portTICK_RATE_MS);
    //     vTaskDelay(5000 / portTICK_RATE_MS);
    // }

    MDF_LOGW("NODE task is exit");

    vTaskDelete(NULL);
}

/**
 * @brief Timed printing system information
 */
static void print_system_info_timercb(void *timer)
{
    uint8_t primary                 = 0;
    wifi_second_chan_t second       = 0;
    mesh_addr_t parent_bssid        = {0};
    uint8_t sta_mac[MWIFI_ADDR_LEN] = {0};
    mesh_assoc_t mesh_assoc         = {0x0};
    wifi_sta_list_t wifi_sta_list   = {0x0};

    esp_wifi_get_mac(ESP_IF_WIFI_STA, sta_mac);
    esp_wifi_ap_get_sta_list(&wifi_sta_list);
    esp_wifi_get_channel(&primary, &second);
    esp_wifi_vnd_mesh_get(&mesh_assoc);
    esp_mesh_get_parent_bssid(&parent_bssid);

    MDF_LOGI("System information, channel: %d, layer: %d, self mac: " MACSTR ", parent bssid: " MACSTR
             ", parent rssi: %d, node num: %d, free heap: %u", primary,
             esp_mesh_get_layer(), MAC2STR(sta_mac), MAC2STR(parent_bssid.addr),
             mesh_assoc.rssi, esp_mesh_get_total_node_num(), esp_get_free_heap_size());

    for (int i = 0; i < wifi_sta_list.num; i++) {
        MDF_LOGI("Child mac: " MACSTR, MAC2STR(wifi_sta_list.sta[i].mac));
    }

#ifdef MEMORY_DEBUG
    if (!heap_caps_check_integrity_all(true)) {
        MDF_LOGE("At least one heap is corrupt");
    }

    mdf_mem_print_heap();
    mdf_mem_print_record();
#endif /**< MEMORY_DEBUG */
}

static mdf_err_t wifi_init()
{
    mdf_err_t ret          = nvs_flash_init();
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();

    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        MDF_ERROR_ASSERT(nvs_flash_erase());
        ret = nvs_flash_init();
    }

    MDF_ERROR_ASSERT(ret);

    tcpip_adapter_init();
    MDF_ERROR_ASSERT(esp_event_loop_init(NULL, NULL));
    MDF_ERROR_ASSERT(esp_wifi_init(&cfg));
    MDF_ERROR_ASSERT(esp_wifi_set_storage(WIFI_STORAGE_FLASH));
    MDF_ERROR_ASSERT(esp_wifi_set_mode(WIFI_MODE_STA));
    MDF_ERROR_ASSERT(esp_wifi_set_ps(WIFI_PS_NONE));
    MDF_ERROR_ASSERT(esp_mesh_set_6m_rate(false));
    MDF_ERROR_ASSERT(esp_wifi_start());

    return MDF_OK;
}

/**
 * @brief All module events will be sent to this task in esp-mdf
 *
 * @Note:
 *     1. Do not block or lengthy operations in the callback function.
 *     2. Do not consume a lot of memory in the callback function.
 *        The task memory of the callback function is only 4KB.
 */
static mdf_err_t event_loop_cb(mdf_event_loop_t event, void *ctx)
{
    MDF_LOGI("event_loop_cb, event: %d", event);

    switch (event) {
        case MDF_EVENT_MWIFI_STARTED:
            MDF_LOGI("MESH is started");
            break;

        case MDF_EVENT_MWIFI_PARENT_CONNECTED:
            MDF_LOGI("Parent is connected on station interface");
            //OLED_P8x16Str(2,4,"Parent connected");
            if_connect=1;
            break;

        case MDF_EVENT_MWIFI_PARENT_DISCONNECTED:
            MDF_LOGI("Parent is disconnected on station interface");
            break;

        case MDF_EVENT_MWIFI_ROUTING_TABLE_ADD:
        case MDF_EVENT_MWIFI_ROUTING_TABLE_REMOVE:
            MDF_LOGI("total_num: %d", esp_mesh_get_total_node_num());
            break;

        case MDF_EVENT_MWIFI_ROOT_GOT_IP: {
            MDF_LOGI("Root obtains the IP address. It is posted by LwIP stack automatically");
            xTaskCreate(tcp_client_write_task, "tcp_client_write_task", 4 * 1024,
                        NULL, CONFIG_MDF_TASK_DEFAULT_PRIOTY, NULL);
            xTaskCreate(tcp_client_read_task, "tcp_server_read", 4 * 1024,
                        NULL, CONFIG_MDF_TASK_DEFAULT_PRIOTY, NULL);
            break;
        }

        default:
            break;
    }

    return MDF_OK;
}

void app_main()
{


    mwifi_init_config_t cfg   = MWIFI_INIT_CONFIG_DEFAULT();
    mwifi_config_t config     = {
        .router_ssid     = CONFIG_ROUTER_SSID,
        .router_password = CONFIG_ROUTER_PASSWORD,
        .mesh_id         = CONFIG_MESH_ID,
        .mesh_password   = CONFIG_MESH_PASSWORD,
    };

    /**
     * @brief Set the log level for serial port printing.
     */
    esp_log_level_set("*", ESP_LOG_INFO);
    esp_log_level_set(TAG, ESP_LOG_DEBUG);

    gpio_pad_select_gpio(CONFIG_LED_GPIO_NUM);
    gpio_set_direction(CONFIG_LED_GPIO_NUM, GPIO_MODE_INPUT_OUTPUT);

    /**
     * @brief Initialize wifi mesh.
     */
    MDF_ERROR_ASSERT(mdf_event_loop_init(event_loop_cb));
    MDF_ERROR_ASSERT(wifi_init());
    MDF_ERROR_ASSERT(mwifi_init(&cfg));
    MDF_ERROR_ASSERT(mwifi_set_config(&config));
    MDF_ERROR_ASSERT(mwifi_start());



    /**
     * @breif Create handler
     */
    xTaskCreate(node_write_task, "node_write_task", 4 * 1024,
                NULL, CONFIG_MDF_TASK_DEFAULT_PRIOTY, NULL);
    xTaskCreate(node_read_task, "node_read_task", 4 * 1024,
                NULL, CONFIG_MDF_TASK_DEFAULT_PRIOTY, NULL);

    TimerHandle_t timer = xTimerCreate("print_system_info", 10000 / portTICK_RATE_MS,
                                       true, NULL, print_system_info_timercb);
    xTimerStart(timer, 0);
}

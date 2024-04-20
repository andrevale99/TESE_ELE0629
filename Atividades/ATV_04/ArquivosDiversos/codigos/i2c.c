#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/i2c.h>
#include <esp_log.h>

static char *TAG = "I2C Scanner";  

//define for i2c 
#define I2C_MASTER_SCL_IO           32                           
#define I2C_MASTER_SDA_IO           33                           
#define I2C_MASTER_FREQ_HZ          400000     
//Pedir ao dispositivo o retorno do ACK                 
#define ACK_CHECK_EN                0x1                         


//main function
void app_main()
{
   	ESP_LOGI(TAG, "Iniciando o I2C ");     

    //Configuração da estrutura para o I2C
	i2c_config_t i2c_config = {
	.mode = I2C_MODE_MASTER,                    
	.sda_io_num = I2C_MASTER_SDA_IO,            
    .sda_pullup_en = GPIO_PULLUP_ENABLE,        
	.scl_io_num = I2C_MASTER_SCL_IO,            
    .scl_pullup_en = GPIO_PULLUP_ENABLE,        
	.master.clk_speed = I2C_MASTER_FREQ_HZ,     
    };

    //Passando os paramentros da configuração e instalando o driver
	i2c_param_config(I2C_NUM_0, &i2c_config);                                             
	i2c_driver_install(I2C_NUM_0, I2C_MODE_MASTER, 0, 0, 0); 

    ESP_LOGI(TAG, "Escaneando por dispositivos no barramento I2C");        

    //O endereço definido com 7 bits, logo 128 possiveis dispositivos
    //Escaneando por todos os endereços e mostrando os endereços com resposta positiva
	for (int addr=0; addr< 128; addr++) 
    {
        //Montando a mensagens a ser enviada
		i2c_cmd_handle_t cmd = i2c_cmd_link_create();                           
		i2c_master_start(cmd);                                                  
		i2c_master_write_byte(cmd, (addr << 1) | I2C_MASTER_WRITE, ACK_CHECK_EN);  
		i2c_master_stop(cmd);                                                   

        //Executando de fato os comandos anteriores e armazenando o retorno positivo em esp_error_code
		esp_err_t esp_error_code = i2c_master_cmd_begin(I2C_NUM_0, cmd, pdMS_TO_TICKS(10));   

        if (esp_error_code == ESP_OK)                            //Verificando os retornos positivos
            ESP_LOGI(TAG, "Endereço encontrado = 0x%x", addr);   //Mostrand os endereços
		i2c_cmd_link_delete(cmd);                                
	}
    ESP_LOGI(TAG, "Escaneamento finalizado");                         
}
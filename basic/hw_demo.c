//the basic method to operate the hw by C 
#include <stdint.h> 

#define read_reg(reg)      (*(volatile unsigned int *)(reg))
#define write_reg(reg, val)    (*(volatile unsigned int *)(reg) = (val))

#define REG_X_ADDRESS   0x20000000
#define BIT_DIV_EN      0x00000001
//usage 1
void hw_init(void) 
{
    unsigned int reg_val; 
    reg_val = read_reg(REG_X_ADDRESS);
    write_reg(REG_X_ADDRESS, reg_val | BIT_DIV_EN);
}

//usage 2  by using C struct 
typedef union {
    uint32_t u32;
    struct
    {
        uint32_t pow_en : 1 ; 
        uint32_t hj_cap : 1 ; 
        uint32_t clk_period : 14; 
        uint32_t rsvd : 16 ; 
    } b;
} HW_I2C_SLAVE_CONFIG;

typedef union {
    uint32_t u32;
    struct
    {
        uint32_t tx_buf_size:4; 
        uint32_t rx_buf_size:4;
        uint32_t cmd_buf_size:4; 
        uint32_t resp_buf_size:4; 
        uint32_t ibi_buf_size :4;
        uint32_t rsvd : 12; 
    } b;
} HW_I2C_QUEUE_SZIE_CAPABILITY; 

typedef struct {
    HW_I2C_SLAVE_CONFIG              salve_config  ;   //1 Offset: 0x0
    HW_I2C_QUEUE_SZIE_CAPABILITY     queue_size  ;  //2 Offset: 0x4
} HW_I2C_s;

#define I2C0_REGS_BASE_ADDR 0x42000000

//usage 
void hw_i2c_init()
{
    //guranteen it's volatile 
    HW_I2C_s volatile *pI2C0_REGS = (HW_I2C_s volatile *)I2C0_REGS_BASE_ADDR;
    //operate by bit area
    pI2C0_REGS->salve_config.b.clk_period = 400000;
    pI2C0_REGS->salve_config.b.pow_en = 1;
    pI2C0_REGS->queue_size.b.cmd_buf_size = 8;
    pI2C0_REGS->queue_size.b.rx_buf_size = 8;
    pI2C0_REGS->queue_size.b.tx_buf_size = 8;
    //read/write by whole registers 
    uint32_t reg_queue_val =  pI2C0_REGS->queue_size.u32; 
    uint32_t reg_slave_config_val  =  pI2C0_REGS->salve_config.u32; 
}
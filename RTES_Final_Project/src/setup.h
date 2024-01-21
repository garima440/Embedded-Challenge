// start of setup.h
#define OUT_X_L 0x28
//register fields(bits): data_rate(2),Bandwidth(2),Power_down(1),Zen(1),Yen(1),Xen(1)
#define CTRL_REG1 0x20
//configuration: 200Hz ODR,50Hz cutoff, Power on, Z on, Y on, X on
#define CTRL_REG1_CONFIG 0b01'10'1'1'1'1
//register fields(bits): reserved(1), endian-ness(1),Full scale sel(2), reserved(1),self-test(2), SPI mode(1)
#define CTRL_REG4 0x23
//configuration: reserved,little endian, 245 dps, reserved,disabled,4-wire mode
#define CTRL_REG4_CONFIG 0b0'0'00'0'00'0

#define SPI_FLAG 1

#define RADIUS 1.1f

#define FILTER_COEFFICIENT 0.9f // Adjust this value as needed

#define PI 3.14159265358979323846

#define BACKGROUND 1
// end of setup.h
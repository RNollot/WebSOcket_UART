deps_config := \
	/c/git/esp-idf/components/app_trace/Kconfig \
	/c/git/esp-idf/components/aws_iot/Kconfig \
	/c/git/esp-idf/components/bt/Kconfig \
	/c/git/esp-idf/components/esp32/Kconfig \
	/c/git/esp-idf/components/ethernet/Kconfig \
	/c/git/esp-idf/components/fatfs/Kconfig \
	/c/git/esp-idf/components/freertos/Kconfig \
	/c/git/esp-idf/components/log/Kconfig \
	/c/git/esp-idf/components/lwip/Kconfig \
	/c/git/esp-idf/components/mbedtls/Kconfig \
	/c/git/esp-idf/components/openssl/Kconfig \
	/c/git/esp-idf/components/spi_flash/Kconfig \
	/c/git/esp-idf/components/bootloader/Kconfig.projbuild \
	/c/git/esp-idf/components/esptool_py/Kconfig.projbuild \
	/c/git/esp-idf/components/partition_table/Kconfig.projbuild \
	/c/git/esp-idf/Kconfig

include/config/auto.conf: \
	$(deps_config)


$(deps_config): ;

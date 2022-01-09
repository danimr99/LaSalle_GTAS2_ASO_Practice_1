#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0x3868185, "module_layout" },
	{ 0xfe990052, "gpio_free" },
	{ 0xc1514a3b, "free_irq" },
	{ 0x92d5838e, "request_threaded_irq" },
	{ 0x84f696ed, "gpiod_direction_output_raw" },
	{ 0x47229b5c, "gpio_request" },
	{ 0xc5850110, "printk" },
	{ 0x6362cb11, "gpiod_set_raw_value" },
	{ 0xb1ad28e0, "__gnu_mcount_nc" },
	{ 0xe06046bd, "gpiod_set_debounce" },
	{ 0xcc43850a, "gpiod_export" },
	{ 0x8be6d30, "gpiod_unexport" },
	{ 0x2db72154, "gpiod_get_raw_value" },
	{ 0x224efce8, "gpiod_to_irq" },
	{ 0xe3e779ad, "gpiod_direction_input" },
	{ 0xc5e1beab, "gpio_to_desc" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "185288545D58713EFA54C36");

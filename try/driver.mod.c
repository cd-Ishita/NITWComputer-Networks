#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;
BUILD_LTO_INFO;

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
	{ 0x7bfe442c, "module_layout" },
	{ 0x6091b333, "unregister_chrdev_region" },
	{ 0xed0a6091, "cdev_del" },
	{ 0xd2e28315, "cdev_add" },
	{ 0x655ff25a, "cdev_init" },
	{ 0xe3ec2f2b, "alloc_chrdev_region" },
	{ 0x6cbbfc54, "__arch_copy_to_user" },
	{ 0xdcb764ad, "memset" },
	{ 0x4b0a3f52, "gic_nonsecure_priorities" },
	{ 0x5c1c3eb4, "cpu_hwcaps" },
	{ 0x12a4e128, "__arch_copy_from_user" },
	{ 0xb788fb30, "gic_pmr_sync" },
	{ 0x5e2d7875, "cpu_hwcap_keys" },
	{ 0x14b89635, "arm64_const_caps_ready" },
	{ 0x9f49dcc4, "__stack_chk_fail" },
	{ 0xc5850110, "printk" },
	{ 0xd36dc10c, "get_random_u32" },
	{ 0x5a8d7975, "kmem_cache_alloc_trace" },
	{ 0x49e617a4, "kmalloc_caches" },
	{ 0x37a0cba, "kfree" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "8BFE1F5992997594E57877A");

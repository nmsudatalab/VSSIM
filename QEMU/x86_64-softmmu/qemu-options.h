
DEFHEADING(Standard options:)

DEF("help", 0, QEMU_OPTION_h,
"-h or -help     display this help and exit\n")

DEF("version", 0, QEMU_OPTION_version,
"-version        display version information and exit\n")

DEF("M", HAS_ARG, QEMU_OPTION_M,
"-M machine      select emulated machine (-M ? for list)\n")

DEF("cpu", HAS_ARG, QEMU_OPTION_cpu,
"-cpu cpu        select CPU (-cpu ? for list)\n")

DEF("smp", HAS_ARG, QEMU_OPTION_smp,
"-smp n          set the number of CPUs to 'n' [default=1]\n")

DEF("numa", HAS_ARG, QEMU_OPTION_numa,
"-numa node[,mem=size][,cpus=cpu[-cpu]][,nodeid=node]\n")

DEF("fda", HAS_ARG, QEMU_OPTION_fda,
"-fda/-fdb file  use 'file' as floppy disk 0/1 image\n")
DEF("fdb", HAS_ARG, QEMU_OPTION_fdb, "")

DEF("hda", HAS_ARG, QEMU_OPTION_hda,
"-hda/-hdb file  use 'file' as IDE hard disk 0/1 image\n")
DEF("hdb", HAS_ARG, QEMU_OPTION_hdb, "")
DEF("hdc", HAS_ARG, QEMU_OPTION_hdc,
"-hdc/-hdd file  use 'file' as IDE hard disk 2/3 image\n")
DEF("hdd", HAS_ARG, QEMU_OPTION_hdd, "")

DEF("cdrom", HAS_ARG, QEMU_OPTION_cdrom,
"-cdrom file     use 'file' as IDE cdrom image (cdrom is ide1 master)\n")

DEF("drive", HAS_ARG, QEMU_OPTION_drive,
"-drive [file=file][,if=type][,bus=n][,unit=m][,media=d][,index=i]\n"
"       [,cyls=c,heads=h,secs=s[,trans=t]][,snapshot=on|off]\n"
"       [,cache=writethrough|writeback|none][,format=f][,serial=s]\n"
"       [,addr=A]\n"
"       [,boot=on|off]\n"
"                use 'file' as a drive image\n")

DEF("mtdblock", HAS_ARG, QEMU_OPTION_mtdblock,
"-mtdblock file  use 'file' as on-board Flash memory image\n")

DEF("sd", HAS_ARG, QEMU_OPTION_sd,
"-sd file        use 'file' as SecureDigital card image\n")

DEF("pflash", HAS_ARG, QEMU_OPTION_pflash,
"-pflash file    use 'file' as a parallel flash image\n")

DEF("boot", HAS_ARG, QEMU_OPTION_boot,
"-boot [order=drives][,once=drives][,menu=on|off]\n"
"                'drives': floppy (a), hard disk (c), CD-ROM (d), network (n)\n")

DEF("snapshot", 0, QEMU_OPTION_snapshot,
"-snapshot       write to temporary files instead of disk image files\n")

DEF("m", HAS_ARG, QEMU_OPTION_m,
"-m megs         set virtual RAM size to megs MB [default=%d]\n")

DEF("k", HAS_ARG, QEMU_OPTION_k,
"-k language     use keyboard layout (for example 'fr' for French)\n")


#ifdef HAS_AUDIO
DEF("audio-help", 0, QEMU_OPTION_audio_help,
"-audio-help     print list of audio drivers and their options\n")
#endif

#ifdef HAS_AUDIO
DEF("soundhw", HAS_ARG, QEMU_OPTION_soundhw,
"-soundhw c1,... enable audio support\n"
"                and only specified sound cards (comma separated list)\n"
"                use -soundhw ? to get the list of supported cards\n"
"                use -soundhw all to enable all of them\n")
#endif


DEF("usb", 0, QEMU_OPTION_usb,
"-usb            enable the USB driver (will be the default soon)\n")

DEF("usbdevice", HAS_ARG, QEMU_OPTION_usbdevice,
"-usbdevice name add the host or guest USB device 'name'\n")

DEF("name", HAS_ARG, QEMU_OPTION_name,
"-name string1[,process=string2]    set the name of the guest\n"
"            string1 sets the window title and string2 the process name (on Linux)\n")

DEF("uuid", HAS_ARG, QEMU_OPTION_uuid,
"-uuid %%08x-%%04x-%%04x-%%04x-%%012x\n"
"                specify machine UUID\n")


DEFHEADING()

DEFHEADING(Display options:)


DEF("nographic", 0, QEMU_OPTION_nographic,
"-nographic      disable graphical output and redirect serial I/Os to console\n")

#ifdef CONFIG_CURSES
DEF("curses", 0, QEMU_OPTION_curses,
"-curses         use a curses/ncurses interface instead of SDL\n")
#endif

#ifdef CONFIG_SDL
DEF("no-frame", 0, QEMU_OPTION_no_frame,
"-no-frame       open SDL window without a frame and window decorations\n")
#endif

#ifdef CONFIG_SDL
DEF("alt-grab", 0, QEMU_OPTION_alt_grab,
"-alt-grab       use Ctrl-Alt-Shift to grab mouse (instead of Ctrl-Alt)\n")
#endif

#ifdef CONFIG_SDL
DEF("no-quit", 0, QEMU_OPTION_no_quit,
"-no-quit        disable SDL window close capability\n")
#endif

#ifdef CONFIG_SDL
DEF("sdl", 0, QEMU_OPTION_sdl,
"-sdl            enable SDL\n")
#endif

DEF("portrait", 0, QEMU_OPTION_portrait,
"-portrait       rotate graphical output 90 deg left (only PXA LCD)\n")

DEF("vga", HAS_ARG, QEMU_OPTION_vga,
"-vga [std|cirrus|vmware|xenfb|none]\n"
"                select video card type\n")

DEF("full-screen", 0, QEMU_OPTION_full_screen,
"-full-screen    start in full screen\n")

#if defined(TARGET_PPC) || defined(TARGET_SPARC)
DEF("g", 1, QEMU_OPTION_g ,
"-g WxH[xDEPTH]  Set the initial graphical resolution and depth\n")
#endif

DEF("vnc", HAS_ARG, QEMU_OPTION_vnc ,
"-vnc display    start a VNC server on display\n")


DEFHEADING()

#ifdef TARGET_I386
DEFHEADING(i386 target only:)
#endif

#ifdef TARGET_I386
DEF("win2k-hack", 0, QEMU_OPTION_win2k_hack,
"-win2k-hack     use it when installing Windows 2000 to avoid a disk full bug\n")
#endif

#ifdef TARGET_I386
DEF("rtc-td-hack", 0, QEMU_OPTION_rtc_td_hack,
"-rtc-td-hack    use it to fix time drift in Windows ACPI HAL\n")
#endif

#ifdef TARGET_I386
DEF("no-fd-bootchk", 0, QEMU_OPTION_no_fd_bootchk,
"-no-fd-bootchk  disable boot signature checking for floppy disks\n")
#endif

#ifdef TARGET_I386
DEF("no-acpi", 0, QEMU_OPTION_no_acpi,
"-no-acpi        disable ACPI\n")
#endif

#ifdef TARGET_I386
DEF("no-hpet", 0, QEMU_OPTION_no_hpet,
"-no-hpet        disable HPET\n")
#endif

#ifdef TARGET_I386
DEF("balloon", HAS_ARG, QEMU_OPTION_balloon,
"-balloon none   disable balloon device\n"
"-balloon virtio[,addr=str]\n"
"                enable virtio balloon device (default)\n")
#endif

#ifdef TARGET_I386
DEF("acpitable", HAS_ARG, QEMU_OPTION_acpitable,
"-acpitable [sig=str][,rev=n][,oem_id=str][,oem_table_id=str][,oem_rev=n][,asl_compiler_id=str][,asl_compiler_rev=n][,data=file1[:file2]...]\n"
"                ACPI table description\n")
#endif

#ifdef TARGET_I386
DEF("smbios", HAS_ARG, QEMU_OPTION_smbios,
"-smbios file=binary\n"
"                Load SMBIOS entry from binary file\n"
"-smbios type=0[,vendor=str][,version=str][,date=str][,release=%%d.%%d]\n"
"                Specify SMBIOS type 0 fields\n"
"-smbios type=1[,manufacturer=str][,product=str][,version=str][,serial=str]\n"
"              [,uuid=uuid][,sku=str][,family=str]\n"
"                Specify SMBIOS type 1 fields\n")
#endif

#ifdef TARGET_I386
DEFHEADING()
#endif

DEFHEADING(Network options:)

#ifdef CONFIG_SLIRP
DEF("tftp", HAS_ARG, QEMU_OPTION_tftp, "")
DEF("bootp", HAS_ARG, QEMU_OPTION_bootp, "")
DEF("redir", HAS_ARG, QEMU_OPTION_redir, "")
#ifndef _WIN32
DEF("smb", HAS_ARG, QEMU_OPTION_smb, "")
#endif
#endif

DEF("net", HAS_ARG, QEMU_OPTION_net,
"-net nic[,vlan=n][,macaddr=mac][,model=type][,name=str][,addr=str][,vectors=v]\n"
"                create a new Network Interface Card and connect it to VLAN 'n'\n"
#ifdef CONFIG_SLIRP
"-net user[,vlan=n][,name=str][,net=addr[/mask]][,host=addr][,restrict=y|n]\n"
"         [,hostname=host][,dhcpstart=addr][,dns=addr][,tftp=dir][,bootfile=f]\n"
"         [,hostfwd=rule][,guestfwd=rule]"
#ifndef _WIN32
"[,smb=dir[,smbserver=addr]]\n"
#endif
"                connect the user mode network stack to VLAN 'n', configure its\n"
"                DHCP server and enabled optional services\n"
#endif
#ifdef _WIN32
"-net tap[,vlan=n][,name=str],ifname=name\n"
"                connect the host TAP network interface to VLAN 'n'\n"
#else
"-net tap[,vlan=n][,name=str][,fd=h][,ifname=name][,script=file][,downscript=dfile]"
#ifdef TUNSETSNDBUF
"[,sndbuf=nbytes]"
#endif
"\n"
"                connect the host TAP network interface to VLAN 'n' and use the\n"
"                network scripts 'file' (default=%s)\n"
"                and 'dfile' (default=%s);\n"
"                use '[down]script=no' to disable script execution;\n"
"                use 'fd=h' to connect to an already opened TAP interface\n"
#ifdef TUNSETSNDBUF
"                use 'sndbuf=nbytes' to limit the size of the send buffer; the\n"
"                default of 'sndbuf=1048576' can be disabled using 'sndbuf=0'\n"
#endif
#endif
"-net socket[,vlan=n][,name=str][,fd=h][,listen=[host]:port][,connect=host:port]\n"
"                connect the vlan 'n' to another VLAN using a socket connection\n"
"-net socket[,vlan=n][,name=str][,fd=h][,mcast=maddr:port]\n"
"                connect the vlan 'n' to multicast maddr and port\n"
#ifdef CONFIG_VDE
"-net vde[,vlan=n][,name=str][,sock=socketpath][,port=n][,group=groupname][,mode=octalmode]\n"
"                connect the vlan 'n' to port 'n' of a vde switch running\n"
"                on host and listening for incoming connections on 'socketpath'.\n"
"                Use group 'groupname' and mode 'octalmode' to change default\n"
"                ownership and permissions for communication port.\n"
#endif
"-net dump[,vlan=n][,file=f][,len=n]\n"
"                dump traffic on vlan 'n' to file 'f' (max n bytes per packet)\n"
"-net none       use it alone to have zero network devices; if no -net option\n"
"                is provided, the default is '-net nic -net user'\n")

DEF("bt", HAS_ARG, QEMU_OPTION_bt, \
"\n" \
"-bt hci,null    dumb bluetooth HCI - doesn't respond to commands\n" \
"-bt hci,host[:id]\n" \
"                use host's HCI with the given name\n" \
"-bt hci[,vlan=n]\n" \
"                emulate a standard HCI in virtual scatternet 'n'\n" \
"-bt vhci[,vlan=n]\n" \
"                add host computer to virtual scatternet 'n' using VHCI\n" \
"-bt device:dev[,vlan=n]\n" \
"                emulate a bluetooth device 'dev' in scatternet 'n'\n")

DEFHEADING()

DEFHEADING(Linux/Multiboot boot specific:)

DEF("kernel", HAS_ARG, QEMU_OPTION_kernel, \
"-kernel bzImage use 'bzImage' as kernel image\n")

DEF("append", HAS_ARG, QEMU_OPTION_append, \
"-append cmdline use 'cmdline' as kernel command line\n")

DEF("initrd", HAS_ARG, QEMU_OPTION_initrd, \
"-initrd file    use 'file' as initial ram disk\n")


DEFHEADING()

DEFHEADING(Debug/Expert options:)


DEF("serial", HAS_ARG, QEMU_OPTION_serial, \
"-serial dev     redirect the serial port to char device 'dev'\n")

DEF("parallel", HAS_ARG, QEMU_OPTION_parallel, \
"-parallel dev   redirect the parallel port to char device 'dev'\n")

DEF("monitor", HAS_ARG, QEMU_OPTION_monitor, \
"-monitor dev    redirect the monitor to char device 'dev'\n")

DEF("pidfile", HAS_ARG, QEMU_OPTION_pidfile, \
"-pidfile file   write PID to 'file'\n")

DEF("singlestep", 0, QEMU_OPTION_singlestep, \
"-singlestep   always run in singlestep mode\n")

DEF("S", 0, QEMU_OPTION_S, \
"-S              freeze CPU at startup (use 'c' to start execution)\n")

DEF("gdb", HAS_ARG, QEMU_OPTION_gdb, \
"-gdb dev        wait for gdb connection on 'dev'\n")

DEF("s", 0, QEMU_OPTION_s, \
"-s              shorthand for -gdb tcp::%s\n")

DEF("d", HAS_ARG, QEMU_OPTION_d, \
"-d item1,...    output log to %s (use -d ? for a list of log items)\n")

DEF("hdachs", HAS_ARG, QEMU_OPTION_hdachs, \
"-hdachs c,h,s[,t]\n" \
"                force hard disk 0 physical geometry and the optional BIOS\n" \
"                translation (t=none or lba) (usually qemu can guess them)\n")

DEF("L", HAS_ARG, QEMU_OPTION_L, \
"-L path         set the directory for the BIOS, VGA BIOS and keymaps\n")

DEF("bios", HAS_ARG, QEMU_OPTION_bios, \
"-bios file      set the filename for the BIOS\n")

#ifdef CONFIG_KQEMU
DEF("kernel-kqemu", 0, QEMU_OPTION_kernel_kqemu, \
"-kernel-kqemu   enable KQEMU full virtualization (default is user mode only)\n")
#endif

#ifdef CONFIG_KQEMU
DEF("enable-kqemu", 0, QEMU_OPTION_enable_kqemu, \
"-enable-kqemu   enable KQEMU kernel module usage\n")
#endif

#ifdef CONFIG_KVM
DEF("enable-kvm", 0, QEMU_OPTION_enable_kvm, \
"-enable-kvm     enable KVM full virtualization support\n")
#endif

#ifdef CONFIG_XEN
DEF("xen-domid", HAS_ARG, QEMU_OPTION_xen_domid,
"-xen-domid id   specify xen guest domain id\n")
DEF("xen-create", 0, QEMU_OPTION_xen_create,
"-xen-create     create domain using xen hypercalls, bypassing xend\n"
"                warning: should not be used when xend is in use\n")
DEF("xen-attach", 0, QEMU_OPTION_xen_attach,
"-xen-attach     attach to existing xen domain\n"
"                xend will use this when starting qemu\n")
#endif

DEF("no-reboot", 0, QEMU_OPTION_no_reboot, \
"-no-reboot      exit instead of rebooting\n")

DEF("no-shutdown", 0, QEMU_OPTION_no_shutdown, \
"-no-shutdown    stop before shutdown\n")

DEF("loadvm", HAS_ARG, QEMU_OPTION_loadvm, \
"-loadvm [tag|id]\n" \
"                start right away with a saved state (loadvm in monitor)\n")

#ifndef _WIN32
DEF("daemonize", 0, QEMU_OPTION_daemonize, \
"-daemonize      daemonize QEMU after initializing\n")
#endif

DEF("option-rom", HAS_ARG, QEMU_OPTION_option_rom, \
"-option-rom rom load a file, rom, into the option ROM space\n")

DEF("clock", HAS_ARG, QEMU_OPTION_clock, \
"-clock          force the use of the given methods for timer alarm.\n" \
"                To see what timers are available use -clock ?\n")

DEF("localtime", 0, QEMU_OPTION_localtime, \
"-localtime      set the real time clock to local time [default=utc]\n")

DEF("startdate", HAS_ARG, QEMU_OPTION_startdate, \
"-startdate      select initial date of the clock\n")

DEF("icount", HAS_ARG, QEMU_OPTION_icount, \
"-icount [N|auto]\n" \
"                enable virtual instruction counter with 2^N clock ticks per\n" \
"                instruction\n")

DEF("watchdog", HAS_ARG, QEMU_OPTION_watchdog, \
"-watchdog i6300esb|ib700\n" \
"                enable virtual hardware watchdog [default=none]\n")

DEF("watchdog-action", HAS_ARG, QEMU_OPTION_watchdog_action, \
"-watchdog-action reset|shutdown|poweroff|pause|debug|none\n" \
"                action when watchdog fires [default=reset]\n")

DEF("echr", HAS_ARG, QEMU_OPTION_echr, \
"-echr chr       set terminal escape character instead of ctrl-a\n")

DEF("virtioconsole", HAS_ARG, QEMU_OPTION_virtiocon, \
"-virtioconsole c\n" \
"                set virtio console\n")

DEF("show-cursor", 0, QEMU_OPTION_show_cursor, \
"-show-cursor    show cursor\n")

DEF("tb-size", HAS_ARG, QEMU_OPTION_tb_size, \
"-tb-size n      set TB size\n")

DEF("incoming", HAS_ARG, QEMU_OPTION_incoming, \
"-incoming p     prepare for incoming migration, listen on port p\n")

#ifndef _WIN32
DEF("chroot", HAS_ARG, QEMU_OPTION_chroot, \
"-chroot dir     Chroot to dir just before starting the VM.\n")
#endif

#ifndef _WIN32
DEF("runas", HAS_ARG, QEMU_OPTION_runas, \
"-runas user     Change to user id user just before starting the VM.\n")
#endif


#if defined(TARGET_SPARC) || defined(TARGET_PPC)
DEF("prom-env", HAS_ARG, QEMU_OPTION_prom_env,
"-prom-env variable=value\n"
"                set OpenBIOS nvram variables\n")
#endif
#if defined(TARGET_ARM) || defined(TARGET_M68K)
DEF("semihosting", 0, QEMU_OPTION_semihosting,
"-semihosting    semihosting mode\n")
#endif
#if defined(TARGET_ARM)
DEF("old-param", 0, QEMU_OPTION_old_param,
"-old-param      old param mode\n")
#endif

DEF("no-kvm", 0, QEMU_OPTION_no_kvm,
"-no-kvm         disable KVM hardware virtualization\n")
DEF("no-kvm-irqchip", 0, QEMU_OPTION_no_kvm_irqchip,
"-no-kvm-irqchip disable KVM kernel mode PIC/IOAPIC/LAPIC\n")
DEF("no-kvm-pit", 0, QEMU_OPTION_no_kvm_pit,
"-no-kvm-pit     disable KVM kernel mode PIT\n")
DEF("no-kvm-pit-reinjection", 0, QEMU_OPTION_no_kvm_pit_reinjection,
"-no-kvm-pit-reinjection disable KVM kernel mode PIT interrupt reinjection\n")
#if defined(TARGET_I386) || defined(TARGET_X86_64) || defined(TARGET_IA64) || defined(__linux__)
DEF("pcidevice", HAS_ARG, QEMU_OPTION_pcidevice,
"-pcidevice host=bus:dev.func[,dma=none][,name=string]\n"
"                expose a PCI device to the guest OS.\n"
"                dma=none: don't perform any dma translations (default is to use an iommu)\n"
"                'string' is used in log output.\n")
#endif
DEF("enable-nesting", 0, QEMU_OPTION_enable_nesting,
"-enable-nesting enable support for running a VM inside the VM (AMD only)\n")
DEF("nvram", HAS_ARG, QEMU_OPTION_nvram,
"-nvram FILE          provide ia64 nvram contents\n")
DEF("tdf", 0, QEMU_OPTION_tdf,
"-tdf                 enable guest time drift compensation\n")
DEF("kvm-shadow-memory", HAS_ARG, QEMU_OPTION_kvm_shadow_memory,
"-kvm-shadow-memory MEGABYTES\n"
"                     allocate MEGABYTES for kvm mmu shadowing\n")
DEF("mem-path", HAS_ARG, QEMU_OPTION_mempath,
"-mem-path FILE       provide backing storage for guest RAM\n")
#ifdef MAP_POPULATE
DEF("mem-prealloc", 0, QEMU_OPTION_mem_prealloc,
"-mem-prealloc        preallocate guest memory (use with -mempath)\n")
#endif

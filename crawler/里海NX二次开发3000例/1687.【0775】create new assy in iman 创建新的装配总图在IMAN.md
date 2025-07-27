### 【0775】create new assy in iman 创建新的装配总图在IMAN

#### 代码

```cpp
    /*HEAD CREATE_NEW_ASSY_IN_IMAN CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_assem.h>  
    #include <uf_part.h>  
    #include <uf_ugmgr.h>  
    #include <uf_modl.h>  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[133];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n+++ ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
        /*  NOTE:  UF_print_syslog is new in V18 译:UF_print_syslog在V18中新增，该函数用于将日志消息发送到系统日志。 */  
            UF_print_syslog(msg, FALSE);  
            UF_print_syslog(err, FALSE);  
            UF_print_syslog("\n", FALSE);  
            UF_print_syslog(call, FALSE);  
            UF_print_syslog(";\n", FALSE);  
            if (!UF_UI_open_listing_window())  
            {  
                UF_UI_write_listing_window(msg);  
                UF_UI_write_listing_window(err);  
                UF_UI_write_listing_window("\n");  
                UF_UI_write_listing_window(call);  
                UF_UI_write_listing_window(";\n");  
            }  
        }  
        return(irc);  
    }  
    static void report_load_status(UF_PART_load_status_p_t status)  
    {  
        char  
            msg[133];  
        int  
            ii;  
        if (status->n_parts > 0)  
        {  
            UF_UI_open_listing_window();  
            UF_UI_write_listing_window("Load notes:\n");  
            for (ii = 0; ii < status->n_parts; ii++)  
            {  
                UF_UI_write_listing_window("  ");  
                UF_UI_write_listing_window(status->file_names[ii]);  
                UF_UI_write_listing_window(" - ");  
                UF_get_fail_message(status->statuses[ii], msg);  
                UF_UI_write_listing_window(msg);  
                UF_UI_write_listing_window("\n");  
            }  
            UF_UI_write_listing_window("\n");  
            UF_free(status->statuses);  
            UF_free_string_array(status->n_parts, status->file_names);  
        }  
    }  
    static void do_it(void)  
    {  
        logical  
            mod;  
        tag_t  
            assy,  
            block,  
            comp,  
            instance,  
            spec;  
        double  
            abs_xyz[6] = { 1,0,0, 0,1,0 },  
            y2[3] = { 0,2,0 },  
            zero[3] = { 0,0,0 };  
        char  
            assy_name[MAX_FSPEC_SIZE+1],  
            comp_name1[MAX_FSPEC_SIZE+1],  
            comp_name2[MAX_FSPEC_SIZE+1],  
            spec_name[MAX_FSPEC_SIZE+1],  
            part_num[UF_UGMGR_PARTNO_SIZE+1],  
            part_rev[UF_UGMGR_PARTREV_SIZE+1],  
            *sizes[3] = { "1", "2", "3" };  
        UF_PART_load_status_t  
            status;  
    /*  Create a component master part and drawing specification 译:创建组件主零件和图纸规范 */  
        UF_CALL(UF_UGMGR_assign_part_number("", "", part_num, &mod));  
        UF_CALL(UF_UGMGR_assign_part_rev(part_num, "", part_rev, &mod));  
        UF_CALL(UF_UGMGR_encode_part_filename(part_num, part_rev, "", "",  
            comp_name1));  
        UF_CALL(UF_PART_new(comp_name1, UF_PART_ENGLISH, &comp));  
        UF_CALL(UF_MODL_create_block(UF_NULLSIGN, NULL_TAG, zero, sizes, &block));  
        UF_CALL(UF_PART_save());  
        UF_CALL(UF_UGMGR_encode_part_filename(part_num, part_rev, "specification",  
            "drawing", spec_name));  
        UF_CALL(UF_PART_new(spec_name, UF_PART_ENGLISH, &spec));  
        UF_CALL(UF_ASSEM_add_part_to_assembly(spec, comp_name1, NULL, NULL, zero,  
            abs_xyz, -1, &instance, &status));  
        report_load_status(&status);  
        UF_CALL(UF_PART_save());  
    /*  Create another component master part and drawing specification 译:创建另一个组件主零件图和绘制规范。 */  
        UF_CALL(UF_UGMGR_assign_part_number("", "", part_num, &mod));  
        UF_CALL(UF_UGMGR_assign_part_rev(part_num, "", part_rev, &mod));  
        UF_CALL(UF_UGMGR_encode_part_filename(part_num, part_rev, "", "",  
            comp_name2));  
        UF_CALL(UF_PART_new(comp_name2, UF_PART_ENGLISH, &comp));  
        UF_CALL(UF_MODL_create_block(UF_NULLSIGN, NULL_TAG, zero, sizes, &block));  
        UF_CALL(UF_PART_save());  
        UF_CALL(UF_UGMGR_encode_part_filename(part_num, part_rev, "specification",  
            "drawing", spec_name));  
        UF_CALL(UF_PART_new(spec_name, UF_PART_ENGLISH, &spec));  
        UF_CALL(UF_ASSEM_add_part_to_assembly(spec, comp_name2, NULL, NULL, zero,  
            abs_xyz, -1, &instance, &status));  
        report_load_status(&status);  
        UF_CALL(UF_PART_save());  
    /*  Create an assembly master and drawing specification 译:创建装配总图和图纸规范 */  
        UF_CALL(UF_UGMGR_assign_part_number("", "", part_num, &mod));  
        UF_CALL(UF_UGMGR_assign_part_rev(part_num, "", part_rev, &mod));  
        UF_CALL(UF_UGMGR_encode_part_filename(part_num, part_rev, "", "",  
            assy_name));  
        UF_CALL(UF_PART_new(assy_name, UF_PART_ENGLISH, &assy));  
        UF_CALL(UF_ASSEM_add_part_to_assembly(assy, comp_name1, NULL, NULL, zero,  
            abs_xyz, -1, &instance, &status));  
        report_load_status(&status);  
        UF_CALL(UF_ASSEM_add_part_to_assembly(assy, comp_name2, NULL, NULL, y2,  
            abs_xyz, -1, &instance, &status));  
        report_load_status(&status);  
        UF_CALL(UF_PART_save());  
        UF_CALL(UF_UGMGR_encode_part_filename(part_num, part_rev, "specification",  
            "drawing", spec_name));  
        UF_CALL(UF_PART_new(spec_name, UF_PART_ENGLISH, &spec));  
        UF_CALL(UF_ASSEM_add_part_to_assembly(spec, assy_name, NULL, NULL, zero,  
            abs_xyz, -1, &instance, &status));  
        report_load_status(&status);  
        UF_CALL(UF_PART_save());  
    }  
    /* 里海 */  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (UF_CALL(UF_initialize())) return;  
        do_it();  
        UF_terminate();  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_IMMEDIATELY);  
    }  
    /*  
        To run in external UG/Manager or IMAN mode:  
            program.exe -pim=yes  
    */  
    int main( int argc, char *argv[] )  
    {  
        if (!UF_CALL(UF_initialize()))  
        {  
            UF_CALL(uc4624(0, argc, argv));  
            do_it();  
            UF_CALL(UF_terminate());  
        }  
        return 0;  
    }

```

#### 代码解析

> 这段NX二次开发代码主要包括以下几个部分：
>
> 1. 头文件包含：代码包含了NX二次开发常用的头文件，如uf.h、uf_ui.h、uf_assem.h、uf_part.h等。
> 2. 错误处理宏定义：通过UF_CALL宏，将NX API调用进行包装，用于检测API调用是否成功，若失败则输出错误信息。
> 3. 错误报告函数：定义了report_error函数，用于在API调用失败时输出错误信息。
> 4. 加载状态报告函数：定义了report_load_status函数，用于输出零件加载状态信息。
> 5. 主体函数：定义了do_it函数，主要完成以下工作：创建两个组件主零件和对应的图纸规范。创建一个装配总图，并将两个组件装配到装配总图中。创建装配总图的图纸规范。
> 6. 创建两个组件主零件和对应的图纸规范。
> 7. 创建一个装配总图，并将两个组件装配到装配总图中。
> 8. 创建装配总图的图纸规范。
> 9. NX启动函数：定义了ufusr函数，用于在NX启动时调用do_it函数。
> 10. 卸载处理函数：定义了ufusr_ask_unload函数，用于在NX卸载时处理。
> 11. 主函数：定义了main函数，用于在命令行模式下启动NX并调用do_it函数。
>
> 总体来说，这段代码通过NX提供的API，实现了创建组件主零件、装配总图和图纸规范的功能，展示了NX二次开发的基本流程。
>

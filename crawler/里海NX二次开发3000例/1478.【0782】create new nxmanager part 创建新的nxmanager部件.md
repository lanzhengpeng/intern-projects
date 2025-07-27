### 【0782】create new nxmanager part 创建新的nxmanager部件

#### 代码

```cpp
    /*HEAD CREATE_NEW_UGMGR_PART CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_ugmgr.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据注释，UF_print_syslog是V18版本新增的函数。 */  
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
    static void do_it(void)  
    {  
        tag_t  
            part;  
        char  
            new_coded[MAX_FSPEC_SIZE+1],  
            seed_coded[MAX_FSPEC_SIZE+1],  
            part_number[UF_UGMGR_PARTNO_SIZE+1],  
            part_revision[UF_UGMGR_PARTREV_SIZE+1],  
            part_file_type[UF_UGMGR_FTYPE_SIZE+1],  
            part_file_name[UF_UGMGR_FNAME_SIZE+1];  
        strcpy(part_number, "000169");  
        strcpy(part_revision, "A");  
        strcpy(part_file_type, "master");  
        strcpy(part_file_name, "");  
        UF_CALL(UF_UGMGR_encode_part_filename( part_number, part_revision,  
           part_file_type, part_file_name, new_coded));  
        strcpy(part_number, "Inch");  
        strcpy(part_revision, "A");  
        strcpy(part_file_type, "");  
        UF_CALL(UF_UGMGR_encode_part_filename( part_number, part_revision,  
           part_file_type, part_file_name, seed_coded));  
        UF_CALL(UF_UGMGR_new_part_from_template(new_coded, "UGMASTER",  
            seed_coded, &part));  
    }  
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

```

#### 代码解析

> 这是段NX二次开发代码，主要功能是从模板创建新的零件。
>
> 关键要点如下：
>
> 1. 头文件包含：代码包含了必要的NX头文件，用于进行NX API调用。
> 2. 错误处理函数：定义了report_error函数，用于打印错误信息到日志和窗口，方便调试。
> 3. 创建新零件的函数：do_it函数实现了从模板创建新零件的主要逻辑，通过调用UF_UGMGR_encode_part_filename函数获取模板文件路径，再调用UF_UGMGR_new_part_from_template函数从模板创建新零件。
> 4. ufusr函数：NX二次开发程序的入口函数，在这里初始化NX环境，调用do_it函数，最后终止NX环境。
> 5. 卸载函数：定义了卸载函数ufusr_ask_unload，返回立即卸载，表示程序退出时会立即卸载。
>
> 总体来说，这段代码实现了从指定模板创建新零件的功能，通过错误处理和日志打印，提高了代码的健壮性和可维护性。
>

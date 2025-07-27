### 【1726】pass udobj class id to ufusr 传递用户定义对象类ID到ufusr函数

#### 代码

```cpp
    /*HEAD PASS_UDOBJ_CLASS_ID_TO_UFUSR CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <ctype.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_udobj.h>  
    #include <uf_obj.h>  
    #include <uf_part.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog在V18中是新增的函数。 */  
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
    DllExport extern UF_UDOBJ_class_t get_my_class_id(void);  
    static tag_t cycle_by_name_prefix(char *prefix, tag_t part, tag_t object,  
        char *fullname)  
    {  
        int  
            ii;  
     /*  make sure the name is all in upper case 里海译:请确保姓名全部使用大写字母。 */  
        if (object == NULL_TAG)  
            for (ii = 0; ii < strlen(prefix); ii++)  
                prefix[ii] = toupper(prefix[ii]);  
        while ((object = UF_OBJ_cycle_all(part, object)) != NULL_TAG)  
            if ((!UF_OBJ_ask_name(object, fullname)) &&  
                (strstr(fullname, prefix) == fullname))  
                break;  
        return object;  
    }  
    static void ask_name_min_max_suffix(char *prefix, int *min, int *max)  
    {  
        int  
            n;  
        tag_t  
            object = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        char  
            fmt[MAX_ENTITY_NAME_SIZE + 1],  
            fullname[MAX_ENTITY_NAME_SIZE + 1];  
        sprintf(fmt, "%s%s", prefix, "%d");  
        *min = 0;  
        *max = 0;  
        while ((object = cycle_by_name_prefix(prefix, part, object, fullname))  
            != NULL_TAG)  
        {  
            sscanf(fullname, fmt, &n);  
            if ((*min == 0) || (n < *min)) *min = n;  
            if (n > *max) *max = n;  
        }  
    }  
    static void do_it(void)  
    {  
        int  
            name_min,  
            name_max;  
        tag_t  
            udo;  
        double  
            pos[3];  
        char  
            msg[133],  
            name[MAX_ENTITY_NAME_SIZE+1],  
            *prefix = { "MY_UDO_" };  
        UF_UDOBJ_class_t  
            my_class_id = get_my_class_id();  
        sprintf(msg, "In ufusr get_my_class_id() = %d\n", my_class_id);  
        UF_print_syslog(msg, FALSE);  
        ask_name_min_max_suffix(prefix, &name_min, &name_max);  
        pos[1] = (double)name_min;  
        pos[2] = 0;  
        UF_CALL(UF_UDOBJ_create_udo(my_class_id, &udo));  
        sprintf(name, "%s%d", prefix, ++name_max);  
        UF_CALL(UF_OBJ_set_name(udo, name));  
        pos[0] = (double)name_max;  
        UF_CALL(UF_OBJ_set_name_origin(udo, pos));  
        UF_CALL(UF_DISP_add_item_to_display(udo));  
    }  
    /* qq3123197280 */  
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

> 这段代码是一个NX的二次开发UF函数，主要功能如下：
>
> 1. 定义了一个错误报告函数report_error，用于打印UF函数调用失败的错误信息。
> 2. 定义了一个按前缀循环查找对象的函数cycle_by_name_prefix。
> 3. 定义了一个按前缀查询对象最小和最大编号的函数ask_name_min_max_suffix。
> 4. 定义了一个主函数do_it，用于创建一个用户自定义对象UDO，并设置其名称和显示在NX图形区。
> 5. 定义了UF函数ufusr，是NX调用的入口，初始化后调用do_it函数，然后结束。
> 6. 定义了一个询问卸载的函数ufusr_ask_unload，返回立即卸载。
>
> 总的来说，这段代码的主要功能是创建一个用户自定义对象UDO，并设置其名称和显示位置，以便在NX图形区中显示。
>

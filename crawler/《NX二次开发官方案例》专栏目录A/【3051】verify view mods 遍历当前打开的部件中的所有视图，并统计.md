### 【3051】verify view mods 遍历当前打开的部件中的所有视图，并统计

#### 代码

```cpp
    /*HEAD VERIFY_VIEW_MODS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_view.h>  
    #include <uf_obj.h>  
    #include <uf_part.h>  
    #include <uf_object_types.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog 是 V18 新增的功能。 */  
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
    static tag_t ask_next_view(tag_t part, tag_t view)  
    {  
        UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_view_type, &view));  
        return (view);  
    }  
    static void do_it(void)  
    {  
        tag_t  
            mod_obj_tag = NULL_TAG,  
            obj_tag = NULL_TAG,  
            part_tag = UF_PART_ask_display_part();  
        int  
            i,  
            nm_length,  
            count;  
        char  
            obj_stat[30],  
            vw_name[30];  
        printf("\n******** Code  Begins here ***************\n");  
        while ((obj_tag = ask_next_view(part_tag, obj_tag)) != NULL_TAG)  
        {  
            UF_CALL(uc5027(obj_tag, vw_name, &nm_length));  
            printf(" view name %s being processed....\n", vw_name);  
            for(i = 2 ; i < 5 ; i ++)  
            {  
                if(i == 2) strcpy(obj_stat,"View dependent");  
                if(i == 3) strcpy(obj_stat,"Erased  object");  
                if(i == 4) strcpy(obj_stat,"Modified object");  
                count = 0;  
                while( !uc6457(vw_name, i, &mod_obj_tag) &&   
                    (mod_obj_tag != NULL_TAG)) count ++;  
                printf("Found %d entities that are %s on %s view\n",  
                        count, obj_stat, vw_name);  
            }  
        }  
    }  
    /* qq3123197280 */  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (!UF_CALL(UF_initialize()))  
        {  
            do_it();  
            UF_CALL(UF_terminate());  
        }  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_IMMEDIATELY);  
    }

```

#### 代码解析

> 这段NX二次开发代码主要实现了以下功能：
>
> 1. 定义了一个错误报告函数report_error，用于输出函数调用失败的相关信息。
> 2. 定义了一个ask_next_view函数，用于遍历当前显示部件中的所有视图，并返回下一个视图的tag。
> 3. 定义了一个do_it函数，用于遍历所有视图，统计不同状态的对象数量，并输出统计结果。
> 4. 定义了ufusr函数，作为用户自定义函数的入口，用于初始化、执行do_it函数，然后终止NX。
> 5. 定义了ufusr_ask_unload函数，用于在卸载用户自定义函数库时立即卸载。
>
> 整体来看，这段代码通过遍历当前显示部件的所有视图，统计不同状态的对象数量，并输出统计结果，实现了对象状态的统计功能。
>

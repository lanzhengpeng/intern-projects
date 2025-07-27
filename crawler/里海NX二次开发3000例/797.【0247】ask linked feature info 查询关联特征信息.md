### 【0247】ask linked feature info 查询关联特征信息

#### 代码

```cpp
    /*HEAD ASK_LINKED_FEATURE_INFO CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_obj.h>  
    #include <uf_modl.h>  
    #include <uf_part.h>  
    #include <uf_wave.h>  
    #include <uf_assem.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #define ECHO(X) { UF_UI_open_listing_window(); \  
        UF_UI_write_listing_window(X); \  
        UF_print_syslog(X, FALSE); }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[UF_UI_MAX_STRING_LEN];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n+++ ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
            ECHO(msg);  
            ECHO(err);  
            ECHO("\n");  
            ECHO(call);  
            ECHO(";\n");  
        }  
        return(irc);  
    }  
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        sprintf(msg, "%s = \"%s\"\n", title, string);  
        ECHO(msg);  
    }  
    static tag_t ask_next_wave_linked_feature(tag_t part, tag_t feat)  
    {  
        char  
            *this_type;  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_feature_type, &feat))  
            && (feat != NULL_TAG))  
        {  
            if (UF_OBJ_ask_status(feat) == UF_OBJ_ALIVE)  
            {  
                UF_CALL(UF_MODL_ask_feat_type(feat, &this_type));  
                if (strstr(this_type, "LINK"))  
                {  
                    UF_free(this_type);  
                    return feat;  
                }  
                UF_free(this_type);  
            }  
        }  
        return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            linked_feat = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        UF_WAVE_linked_feature_info_t   
            name_store;  
        while((linked_feat =ask_next_wave_linked_feature(part, linked_feat))!=NULL_TAG)  
        {  
            UF_CALL( UF_WAVE_init_linked_feature_info( &name_store ));  
            UF_CALL( UF_WAVE_ask_linked_feature_info( linked_feat, &name_store));  
            WRITE_S( name_store.feature_name );  
            WRITE_S( name_store.owning_part_name );  
            WRITE_S( name_store.source_part_name );  
            WRITE( "===========================================\n" );  
            UF_CALL( UF_WAVE_free_linked_feature_info( &name_store ));  
        }  
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

```

#### 代码解析

> 这段代码是一个NX Open的二次开发示例，其主要功能是遍历当前显示的零件，并查询所有具有“LINK”特征的关联特征信息。
>
> 主要步骤包括：
>
> 1. 初始化NX系统，通过UF_initialize函数。
> 2. 获取当前显示的零件，通过UF_PART_ask_display_part函数。
> 3. 循环遍历零件中的所有特征，通过UF_OBJ_cycle_objs_in_part函数，并查找具有“LINK”特征的特征。
> 4. 对于每个“LINK”特征，查询其关联特征信息，包括特征名称、拥有零件名称和源零件名称，通过UF_WAVE_linked_feature_info结构体和UF_WAVE_ask_linked_feature_info函数。
> 5. 打印每个“LINK”特征的关联特征信息。
> 6. 释放关联特征信息内存，通过UF_WAVE_free_linked_feature_info函数。
> 7. 终止NX系统，通过UF_terminate函数。
>
> 此外，代码中还包含错误处理和日志打印的辅助函数。
>
> 该代码主要用于演示如何在NX中查询和打印关联特征信息，为NX Open的二次开发提供参考。
>

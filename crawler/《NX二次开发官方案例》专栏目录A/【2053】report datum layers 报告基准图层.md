### 【2053】report datum layers 报告基准图层

#### 代码

```cpp
    /*HEAD REPORT_DATUM_LAYERS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_modl.h>  
    #include <uf_obj.h>  
    #include <uf_assem.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
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
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    #define WRITENZ(X) if (X != 0) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = \"%s\"\n", title, string);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static tag_t ask_next_feature(tag_t part, tag_t feat)  
    {  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_feature_type, &feat))  
            && (feat != NULL_TAG))  
            if (UF_OBJ_ask_status(feat) == UF_OBJ_ALIVE) return feat;  
        return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        int  
            num = 0,  
            inx = 0;  
        tag_t  
            this_feat = NULL_TAG,  
            *ents = NULL,  
            part = UF_ASSEM_ask_work_part();  
        char  
            *this_type;  
        UF_OBJ_disp_props_t  
            disp_props;  
        while ((this_feat = ask_next_feature(part, this_feat)) != NULL_TAG)  
        {  
            WRITE_D( this_feat );  
            UF_CALL(UF_MODL_ask_feat_type(this_feat, &this_type));  
            if ( strstr( this_type, "DATUM" ))  
            {  
                WRITE_S( this_type );  
                UF_free( this_type );  
                UF_CALL(UF_MODL_ask_feat_object(this_feat, &num, &ents));  
                for( inx = 0; inx < num; inx++ )  
                {  
                    UF_CALL(UF_OBJ_ask_display_properties(ents[inx],   
                                                        &disp_props));  
                    WRITE_D( inx );  
                    WRITE_D( disp_props.layer );  
                }  
                if( num ) UF_free(ents);  
            }  
            else  
            {  
                UF_free(this_type);  
                WRITE( "==========\n");  
            }  
        }  
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

> 这是一段用于NX的二次开发代码，其主要功能是遍历当前工作部件中的所有特征，并输出包含“DATUM”的特征类型以及它们对应的图层号。
>
> 主要步骤如下：
>
> 1. 引入必要的NX二次开发头文件。
> 2. 定义宏函数UF_CALL，用于调用NX API，并在出错时输出错误信息。
> 3. 定义宏函数WRITE，用于输出信息到列表窗口。
> 4. 定义函数write_integer_to_listing_window和write_string_to_listing_window，用于输出整数和字符串信息到列表窗口。
> 5. 定义函数ask_next_feature，用于遍历部件中的所有特征。
> 6. 定义函数do_it，用于遍历所有特征，判断特征类型是否包含“DATUM”，如果是，则输出特征类型和对应的图层号。
> 7. 定义ufusr函数，用于初始化NX，调用do_it函数，然后终止NX。
> 8. 定义ufusr_ask_unload函数，用于卸载用户函数模块。
>
> 这段代码的主要目的是输出工作部件中所有基准特征的类型和图层号，为后续的编程操作提供信息参考。
>

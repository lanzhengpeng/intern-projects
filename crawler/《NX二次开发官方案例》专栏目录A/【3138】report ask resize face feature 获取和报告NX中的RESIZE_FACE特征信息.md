### 【3138】report ask resize face feature 获取和报告NX中的RESIZE_FACE特征信息

#### 代码

```cpp
    /*HEAD REPORT_ASK_RESIZE_FACE_FEATURE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_modl.h>  
    #include <uf_modl_dfo.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    /* qq3123197280 */  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[133];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n+++ ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译：注意，UF_print_syslog是V18版本新增的。 */  
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
    /* qq3123197280 */  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    /* qq3123197280 */  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = \"%s\"\n", title, string);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    /* qq3123197280 */  
    static tag_t select_a_feature(char *prompt)  
    {  
        int  
            cnt,  
            resp;  
        tag_t  
            first,  
            *feats;  
        UF_CALL(UF_UI_select_feature(prompt, NULL, &cnt, &feats, &resp));  
        if (cnt)  
        {  
            first = feats[0];  
            UF_free(feats);  
            return (first);  
        }  
        else return (NULL_TAG);  
    }  
    /* qq3123197280 */  
    static void do_it(void)  
    {  
        int  
            inx = 0,  
            num_target = 0,  
            num_non_blend = 0;  
        tag_t  
            feat = NULL_TAG,  
            *target_faces = NULL,  
            *non_blend_faces = NULL;  
        char       
            *parameter = NULL,  
            *feature_type = NULL;  
            if( (feat = select_a_feature("Get RESIZE_FACE feature")) != NULL_TAG )  
            {  
                if (!UF_CALL(UF_MODL_ask_feat_type( feat, &feature_type )))  
                {  
                    WRITE_S(feature_type);  
                    if( strstr( feature_type, "RESIZE_FACE" ) != NULL )  
                    {  
                        UF_CALL( UF_MODL_ask_resize_face( feat, &target_faces,   
                                                &num_target, &non_blend_faces,  
                                                    &num_non_blend, &parameter ));  
                        WRITE_D( num_target );  
                        WRITE_D( num_non_blend );  
                        WRITE_S( parameter );  
                        for( inx = 0; inx < num_target; inx++ )  
                        {  
                            WRITE_D( target_faces[inx] );     
                        }  
                        for( inx = 0; inx < num_non_blend; inx++ )  
                        {  
                            WRITE_D( non_blend_faces[inx] );     
                        }  
                        if( num_target ) UF_free( target_faces );  
                        if( num_non_blend ) UF_free( non_blend_faces );  
                    }  
                    UF_free(feature_type);  
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

> 这段代码是NX Open C++ API的二次开发代码，主要功能是获取选定特征的面缩放参数。具体来说，代码实现了以下功能：
>
> 1. 定义了错误报告函数report_error，用于输出错误信息。
> 2. 定义了整数和字符串输出函数write_integer_to_listing_window和write_string_to_listing_window，用于向列表窗口输出信息。
> 3. 定义了选择特征函数select_a_feature，用于提示用户选择一个特征。
> 4. 定义了执行函数do_it，该函数获取用户选择特征的面缩放参数，包括目标面和非融合面列表，以及参数字符串，并将这些信息输出到列表窗口。
> 5. 定义了ufusr函数，作为程序的入口函数，初始化并调用do_it函数，最后终止。
> 6. 定义了卸载函数ufusr_ask_unload，用于立即卸载二次开发代码。
> 7. 使用NX Open C++ API中的UF模块，如UF_MODL，实现特征操作。
>
> 综上所述，这段代码实现了选择一个特征，获取其面缩放参数并输出这些信息的功能。
>

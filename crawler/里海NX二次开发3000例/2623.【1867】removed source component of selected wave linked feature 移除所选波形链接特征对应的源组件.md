### 【1867】removed source component of selected wave linked feature 移除所选波形链接特征对应的源组件

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_so.h>  
    #include <uf_assem.h>  
    #include <uf_wave.h>  
    #include <uf_obj.h>  
    #include <uf_ui.h>  
    #include <stdarg.h>  
    static void ECHO(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_LEN+1];  
        va_list args;  
        va_start(args, format);  
        vsprintf(msg, format, args);  
        va_end(args);  
        UF_UI_open_listing_window();  
        UF_UI_write_listing_window(msg);  
        UF_print_syslog(msg, FALSE);  
    }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133];  
            UF_get_fail_message(irc, err);  
            ECHO("*** ERROR code %d at line %d in %s:\n",  
                irc, line, file);  
            ECHO("+++ %s\n", err);  
            ECHO("%s;\n", call);  
        }  
        return(irc);  
    }  
    static void GTAC_print_syslog(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_LEN+1];  
        va_list args;  
        va_start(args, format);  
        vsprintf(msg, format, args);  
        va_end(args);  
        UF_print_syslog(msg, FALSE);  
    }  
    static void GTAC_uc1601(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_LEN+1];  
        va_list args;  
        va_start(args, format);  
        vsprintf(msg, format, args);  
        va_end(args);  
        uc1601(msg, TRUE);  
    }  
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
    static void do_it(void)  
    {  
        tag_t  
            linkedFeat = NULL_TAG,  
            xform = NULL_TAG,  
            fromPartOcc = NULL_TAG,  
            instance = NULL_TAG,  
            rootOcc = UF_ASSEM_ask_root_part_occ(UF_PART_ask_display_part());  
        linkedFeat = select_a_feature("Linked Feature:");  
        UF_CALL( UF_WAVE_ask_link_xform( linkedFeat, &xform ));  
        UF_CALL(UF_SO_ask_assy_ctxt_part_occ( xform, rootOcc, &fromPartOcc ));  
        instance = UF_ASSEM_ask_inst_of_part_occ( fromPartOcc );  
        UF_CALL( UF_ASSEM_remove_instance( instance ));  
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

> 这是段NX二次开发代码，主要功能是从装配体中删除一个特征实例。代码使用UF函数来实现以下功能：
>
> 1. 初始化NX环境，使用UF_initialize()。
> 2. 提示用户选择一个特征，使用UF_UI_select_feature()。
> 3. 获取该特征对应的变换矩阵，使用UF_WAVE_ask_link_xform()。
> 4. 获取该特征所在组件的实例，使用UF_SO_ask_assy_ctxt_part_occ()和UF_ASSEM_ask_inst_of_part_occ()。
> 5. 从装配体中删除该特征实例，使用UF_ASSEM_remove_instance()。
> 6. 最后，清理NX环境，使用UF_terminate()。
>
> 此外，代码还定义了两个宏：
>
> 此外，还包含了一个错误处理函数report_error()，用于处理UF函数调用错误。
>
> 整体来看，这段代码实现了从NX装配体中删除特征实例的功能。
>

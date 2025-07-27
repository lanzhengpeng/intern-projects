### 【0983】create uniform scale feature of specified scale at wcs 创建统一缩放特征于指定比例在WCS

#### 代码

```cpp
    /*HEAD CREATE_UNIFORM_SCALE_FEATURE_OF_SPECIFIED_SCALE_AT_WCS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_modl.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，可以翻译如下：

注意：UF_print_syslog 是 V18 版本新增的。 */  
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
    /* qq3123197280 */  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    /* qq3123197280 */  
    static int mask_for_bodies(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_BODY };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    /* qq3123197280 */  
    static int select_bodies(char *prompt, tag_t **bodies)  
    {  
        int  
            cnt,  
            ii,  
            resp;  
        UF_CALL(UF_UI_select_with_class_dialog("Select solids or sheets", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_bodies, NULL, &resp, &cnt,  
            bodies));  
        for (ii = 0; ii < cnt; ii++)  
            UF_CALL(UF_DISP_set_highlight((*bodies)[ii], FALSE));  
        return cnt;  
    }  
    /* qq3123197280 */  
    static logical prompt_for_a_number(char *prompt, char *item, double *number)  
    {  
        int  
            irc,  
            resp;  
        char  
            menu[1][16];  
        double  
            da[1];  
        strncpy(&menu[0][0], item, 16);  
        menu[0][15] = '\0';  
        da[0] = *number;  
        resp = uc1609(prompt, menu, 1, da, &irc);  
        if (resp == 3 || resp == 4)  
        {  
            *number = da[0];  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    /* qq3123197280 */  
    static void do_it(void)  
    {  
        int  
            inx = 0,  
            num_bodies = 0;  
        tag_t  
            * body_tags = NULL,  
            *tags = NULL;  
        char   
            factors[ 3 ][ 256 ] = { "", "", "", };  
        double  
            new_scale = 0.;  
        while((( num_bodies = select_bodies("Select bodies", &body_tags)) > 0 ) &&  
               ( prompt_for_a_number("New Scale?", "new Scale:", &new_scale)))  
        {  
            sprintf(factors[0], "%f", new_scale );  
            UF_CALL( UF_MODL_create_scale( UF_SCALE_TYPE_UNIFORM, body_tags,  
                                           num_bodies, NULL_TAG, NULL_TAG,   
                                           NULL_TAG, factors,  &tags ));  
            WRITE( "New tags for scale feature bodies:\n" );  
            for( inx = 0; inx < num_bodies; inx++ )  
            {  
                WRITE_D( tags[inx] );  
            }  
            UF_free( body_tags );  
            UF_free( tags );  
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

> 这段代码是用于NX的二次开发，主要功能是创建统一缩放特征。具体来说：
>
> 1. 代码首先包含所需的头文件，并定义了一个错误报告函数report_error，用于输出错误信息。
> 2. 定义了两个宏WRITE和WRITE_D，分别用于打开列表窗口并写入字符串和整数。
> 3. 定义了一个mask_for_bodies函数，用于设置选择遮罩，以选择实体。
> 4. 定义了一个select_bodies函数，用于提示用户选择实体，并返回选择的实体数量和tag数组。
> 5. 定义了一个prompt_for_a_number函数，用于提示用户输入一个数。
> 6. 主函数do_it中，循环调用select_bodies和prompt_for_a_number，获取用户选择的实体和缩放比例，然后调用UF_MODL_create_scale接口创建统一缩放特征，并输出新生成的特征body的tag。
> 7. ufusr函数是NX二次开发程序的入口函数，其中初始化NX环境，调用do_it执行主要功能，最后终止NX环境。
> 8. ufusr_ask_unload函数用于设置程序卸载方式。
>
> 总体来说，这段代码实现了统一缩放特征创建的功能，用户可以交互选择实体并输入缩放比例，程序会创建对应的特征。
>

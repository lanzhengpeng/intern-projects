### 【3089】create resize face from selected cylindrical or spherical faces 从选择的圆柱面或球面创建一个调整大小的面

#### 代码

```cpp
    /*HEAD CREATE_RESIZE_FACE_FROM_SELECTED_CYLINDRICAL_OR_SPHERICAL_FACES CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #include <uf_modl_dfo.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档，UF_print_syslog是V18版本中的新功能，用于打印系统日志。 */  
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
    /* 里海 */  
    static int mask_for_faces(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_FACE };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    /* 里海 */  
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
    /* 里海 */  
    static int select_faces(char *prompt, tag_t **faces)  
    {  
        int  
            cnt,  
            ii,  
            resp;  
        UF_CALL(UF_UI_select_with_class_dialog("Select faces", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_faces, NULL, &resp,  
            &cnt, faces));  
        for (ii = 0; ii < cnt; ii++)  
            UF_CALL(UF_DISP_set_highlight((*faces)[ii], FALSE));  
        return cnt;  
    }  
    /* 里海 */  
    static void do_it(void)  
    {  
        int  
            inx = 0,  
            num_adj_faces = 0,  
            num_target_faces = 0;  
        tag_t  
            *target_faces = NULL,  
            resize_feature = NULL_TAG,  
            *adj_faces = NULL,  
            initial_body = NULL_TAG,  
            comparison_body = NULL_TAG;  
        double  
            diameter = 0.0;  
        char  
            new_parameter[132] = { "" };  
        while ( (num_target_faces = select_faces("Select Target Faces",   
                                               &target_faces)) != 0 )  
        {  
            UF_CALL( UF_MODL_ask_face_body( target_faces[0], &initial_body ));  
            if( num_target_faces > 1 )   
            {  
                for( inx = 1; inx < num_target_faces; inx++ )  
                {  
                    UF_CALL( UF_MODL_ask_face_body( target_faces[inx],   
                                                     &comparison_body ));  
                    if( comparison_body != initial_body )  
                    {  
                        UF_free( target_faces );  
                        WRITE( "All of the faces must belong to\n" );  
                        WRITE( "the same body to create a RESIZE_FACE feature\n\n" );  
                        return;  
                    }  
                }  
            }  
            do  
            {  
                prompt_for_a_number("New Diameter:", "diameter", &diameter);  
            } while ( diameter <= 0. );  
            sprintf( new_parameter, "%f", diameter );  
            num_adj_faces = select_faces("Adjacent (non-blend) Faces",   
                                                                    &adj_faces);  
            UF_CALL( UF_MODL_create_resize_face ( target_faces, num_target_faces,   
                     adj_faces, num_adj_faces, new_parameter, &resize_feature ));  
            if( num_target_faces > 0 ) UF_free( target_faces );  
            if( num_adj_faces > 0 ) UF_free( adj_faces );  
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

> 这段代码是NX的二次开发代码，主要实现了选择圆柱面或球面，然后根据用户输入的直径创建或修改一个调整特征的功能。代码主要包括以下几个部分：
>
> 1. 错误报告函数：用于在函数执行失败时打印错误信息。
> 2. 创建调整特征：首先选择圆柱面或球面作为目标面，然后选择相邻的非过渡面，最后根据用户输入的直径创建调整特征。
> 3. 提示输入直径：提示用户输入新的直径值，并验证输入值是否大于0。
> 4. 主体函数：调用选择面函数，根据选择的面创建调整特征。
> 5. ufusr函数：NX二次开发程序入口函数，初始化NX环境，调用主体函数，最后结束NX环境。
> 6. 卸载函数：返回立即卸载标志，表示在调用ufusr函数后立即卸载用户自定义函数。
>
> 整体上，这段代码实现了在NX中通过选择圆柱面或球面，并输入新的直径值，创建或修改调整特征的功能。代码结构清晰，注释详细，便于理解和修改。
>

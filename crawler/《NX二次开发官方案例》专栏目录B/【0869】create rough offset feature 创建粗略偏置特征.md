### 【0869】create rough offset feature 创建粗略偏置特征

#### 代码

```cpp
    /*****************************************************************************  
    **  
    ** create_rough_offset_feature.c  
    **  
    ** Description:  
    **     Contains Unigraphics entry points for the application.  
    **  
    *****************************************************************************/  
    /* Include files 译:Include files */  
    #include <stdarg.h>  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <uf.h>  
    #include <uf_disp.h>  
    #include <uf_modl.h>  
    #include <uf_ui_types.h>  
    #include <uf_ui.h>  
    static void ECHO(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_BUFSIZE];  
        va_list args;  
        va_start(args, format);  
        vsnprintf_s(msg, sizeof(msg), _TRUNCATE, format, args);  
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
    static logical ask_yes_or_no(char *title, char *message)  
    {  
        int  
            resp;  
        UF_UI_message_buttons_t  
            buttons = { TRUE, FALSE, TRUE, "Yes", NULL, "No", 1, 0, 2 };  
        UF_CALL(UF_UI_message_dialog(title, UF_UI_MESSAGE_QUESTION, &message, 1,  
            TRUE, &buttons, &resp));  
        if (resp == 1) return TRUE;  
        else return FALSE;  
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
    static int mask_for_face_sheet_facet(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[3] = {  
                { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_FACE },  
                { UF_solid_type, 0, UF_UI_SEL_FEATURE_SHEET_BODY },  
                { UF_faceted_model_type, 0, 0 }  
        };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
            UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 3, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_face_sheet_facet(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog(prompt, prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_face_sheet_facet, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void report_rough_offset_parms(UF_MODL_rough_offset_t mParms)  
    {  
        ECHO(" Number of Entities: %d\n", mParms.num_entities);  
        for (int ii = 0; ii<mParms.num_entities; ii++)  
            ECHO(" Entities: %d\n", mParms.entities[ii]);  
        ECHO(" Coordinate System: %d\n", mParms.smart_csys);  
        ECHO(" Offset Distance: %s\n", mParms.offset_distance);  
        ECHO(" Offset Deviation: %s\n", mParms.offset_deviation);  
        ECHO(" Stepover Distance: %s\n", mParms.stepover_distance);  
        ECHO(" Surf method: %d\n", mParms.surf_method);  
        ECHO(" Surface control type: %d\n", mParms.surf_ctrl_type);  
        ECHO(" U patches: %d\n", mParms.u_patches);  
        ECHO(" Boundary trimming option: %d\n", mParms.boundary_trim);  
        UF_UI_update_listing_window();  
    }  
    static void do_it(void)  
    {  
        tag_t tFace, tFeatOld, tFeatNew;  
        UF_MODL_rough_offset_t mParms;  
        char *sType, *sName;  
        if (ask_yes_or_no("Rough Offset Data", "Use data from existing feature?") &&   
            (tFeatOld = select_a_feature("Select rough feature to query")) != NULL_TAG)  
        {  
            UF_CALL(UF_MODL_ask_feat_name(tFeatOld, &sName));  
            UF_CALL(UF_MODL_ask_feat_type(tFeatOld, &sType));  
            ECHO("\nFeature: %s - %s\n", sName, sType);  
            UF_free(sName);  
            if (strcmp(sType, "ROUGH_OFFSET") == 0)  
            {  
                UF_CALL(UF_MODL_ask_rough_offset(tFeatOld, &mParms));  
                report_rough_offset_parms(mParms);  
                UF_CALL(UF_MODL_create_rough_offset(&mParms, &tFeatNew));  
                ECHO("\nNew Feature: %d\n", tFeatNew);  
                UF_MODL_free_rough_offset_data(&mParms);  
            }  
            else  
            {  
                ECHO("\nSelected Feature is not a ROUGH_OFFSET. Exit.\n");  
                return;  
            }  
        }  
        else  
        {  
            if ((tFace = select_a_face_sheet_facet("Select Face/SheetBody/FacetBody")) != NULL_TAG)  
            {  
                tag_t entities[1] = { tFace };  
                sprintf_s(mParms.offset_distance, UF_MAX_EXP_BUFSIZE, "0");  
                sprintf_s(mParms.offset_deviation, UF_MAX_EXP_BUFSIZE, "1");  
                sprintf_s(mParms.stepover_distance, UF_MAX_EXP_BUFSIZE, "2");  
                mParms.boundary_trim = UF_RSO_TRIM_OPT_NO;  
                mParms.entities = entities;  
                mParms.num_entities = 1;  
                mParms.smart_csys = NULL_TAG;  
                mParms.surf_ctrl_type = UF_RSO_SURF_CTRL_BY_USER;  
                mParms.surf_method = UF_RSO_SURF_METHOD_ROUGH_FIT;  
                mParms.u_patches = 1;  
                report_rough_offset_parms(mParms);  
                UF_CALL(UF_MODL_create_rough_offset(&mParms, &tFeatNew));  
                ECHO("\nNew Feature: %d\n", tFeatNew);  
                /*   
                Calling UF_MODL_free_rough_offset_data would fail here because the  
                tag_t array was not allocated (as done by UF_MODL_ask_rough_offset)  
                */  
            }  
        }  
    }  
    /*****************************************************************************  
    **  Activation Methods  
    *****************************************************************************/  
    /*  Explicit Activation  
    **      This entry point is used to activate the application explicitly, as in  
    **      "File->Execute UG/Open->User Function..." */  
    extern DllExport void ufusr( char *parm, int *returnCode, int rlen )  
    {  
        /* Initialize the API environment 译:初始化API环境 */  
        if (UF_CALL(UF_initialize())) return;  
        do_it();  
        /* Terminate the API environment 译:终止API环境 */  
        UF_CALL(UF_terminate());  
    }  
    /*****************************************************************************  
    **  Utilities  
    *****************************************************************************/  
    /* Unload Handler  
    **     This function specifies when to unload your application from Unigraphics.  
    **     If your application registers a callback (from a MenuScript item or a  
    **     User Defined Object for example), this function MUST return  
    **     "UF_UNLOAD_UG_TERMINATE". */  
    extern int ufusr_ask_unload( void )  
    {  
        return( UF_UNLOAD_IMMEDIATELY );  
    }

```

#### 代码解析

> 这段代码是用于NX的二次开发，主要实现了粗糙偏置特征的创建。
>
> 代码的主要功能包括：
>
> 1. 提供了两种创建粗糙偏置特征的方法：一种是通过选择已有的粗糙偏置特征，然后查询其参数并创建一个新的粗糙偏置特征；另一种是选择一个面、薄片体或网格体，然后指定参数来创建一个新的粗糙偏置特征。
> 2. 使用UF_MODL_create_rough_offset函数来创建新的粗糙偏置特征。
> 3. 使用UF_UI等函数实现用户交互，如消息提示框、选择特征等。
> 4. 使用UF_get_fail_message等函数来报告错误。
> 5. 提供了两个激活方法：一个是显式激活方法ufusr，另一个是卸载方法ufusr_ask_unload。
> 6. 使用了许多标准宏和函数来处理API调用，如UF_CALL、report_error等。
> 7. 输出了一些调试信息到日志窗口。
>
> 总体来说，这段代码实现了粗糙偏置特征的基本创建功能，并考虑到了用户交互和错误处理，是一个典型的NX二次开发示例。
>

### 【0800】create offset midsurface and report thickness 创建偏移中面并报告厚度

#### 代码

```cpp
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_disp.h>  
    #include <uf_sf.h>  
    #include <uf_modl.h>  
    #include <uf_part.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <stdarg.h>  
    static void ECHO(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_LEN+1];  
        va_list args;  
        va_start(args, format);  
        vsnprintf_s(msg, sizeof(msg), UF_UI_MAX_STRING_LEN, format, args);  
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
    static int select_faces(char *prompt, tag_t **faces)  
    {  
        int  
            cnt,  
            ii,  
            resp;  
        tag_t  
            *objs;  
        UF_CALL(UF_UI_select_with_class_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_WORK_PART, mask_for_faces, NULL, &resp, &cnt, faces));  
        objs = *faces;  
        for (ii = 0; ii < cnt; ii++)  
            UF_CALL(UF_DISP_set_highlight(objs[ii], FALSE));  
        return cnt;  
    }  
    static tag_t create_offset_midsrf_feature(tag_t seed_face, tag_t feat)  
    {  
        double  
            cliff_angle = 5.0,  // best practice = 5, max suggested = 15  
            percentage_dist = 50.0;  
        UF_CALL(UF_SF_create_offset_midsrf(seed_face, cliff_angle, percentage_dist,  
                                           &feat));  
        return feat;  
    }  
    static void report_offset_midsrf_thickness( tag_t feat_midsrf)  
    {  
        double  
            thickness;  
        char  
            msg[300] = { "" };  
        UF_SF_ask_offset_midsrf_thickness(feat_midsrf, &thickness);  
        ECHO( "thickness of offset midsurface: %f\n", thickness);  
    }  
    /* 里海 */  
    static void do_it(void)  
    {  
        int  
            ii,  
            n;  
        tag_t  
            *faces,  
            feat=NULL_TAG;  
        while ((n = select_faces("Select faces", &faces)) != 0)  
        {  
            for (ii = 0; ii < n; ii++)  
            {  
                ECHO( "seed face: %d\n", faces[ii]);  
                feat = create_offset_midsrf_feature(faces[ii], feat);  
                if (feat != NULL_TAG)  
                    report_offset_midsrf_thickness(feat);  
            }  
            UF_free(faces);  
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

> 这段代码是一个NX二次开发的示例，其主要功能是创建偏移中间曲面特征。具体来说，代码实现了以下功能：
>
> 1. 定义了错误报告函数report_error，用于输出错误信息。
> 2. 定义了面选择函数select_faces，用于提示用户选择面，并返回选择的面的tag数组。
> 3. 定义了创建偏移中间曲面特征的函数create_offset_midsrf_feature，接收种子面和特征，创建偏移中间曲面特征并返回。
> 4. 定义了查询偏移中间曲面特征厚度的函数report_offset_midsrf_thickness，接收特征，并输出其厚度。
> 5. 定义了主函数do_it，循环调用select_faces选择面，对每个面调用create_offset_midsrf_feature创建偏移中间曲面特征，并调用report_offset_midsrf_thickness输出厚度。
> 6. 定义了ufusr函数，初始化NX环境，调用do_it，然后终止NX环境。
> 7. 定义了卸载函数ufusr_ask_unload，返回立即卸载标志。
>
> 总体来说，这段代码通过NX提供的UF函数，实现了偏移中间曲面特征的创建和查询厚度功能。
>

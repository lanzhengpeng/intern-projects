### 【2290】report reblend face data 该程序用于报告重熔面数据，包括名称、类型、系统名称、半径、链信息等

#### 代码

```cpp
    /*HEAD REPORT_REBLEND_FACE_DATA CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_modl.h>  
    #include <uf_modl_dfo.h>  
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
    #define WRITE_L(X) (write_logical_to_listing_window(#X, X))  
    static void write_logical_to_listing_window(char *title, logical flag)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        if (flag)  
            sprintf(msg, "%s = TRUE\n", title);  
        else  
            sprintf(msg, "%s = FALSE\n", title);  
        ECHO(msg);  
    }  
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        sprintf(msg, "%s = %d\n", title, n);  
        ECHO(msg);  
    }  
    #define WRITEnT(n,X) (write_tag_array_to_listing_window(#X, X, 1, n))  
    static void write_tag_array_to_listing_window(char *name, tag_t *array,  
            int rows, int cols)  
    {  
        int  
            ii,  
            jj,  
            kk = 0;  
        char  
            blanks[UF_UI_MAX_STRING_LEN+1],  
            msg[UF_UI_MAX_STRING_LEN+1];  
        memset(blanks, ' ', strlen(name) + 3);  
        blanks[strlen(name) + 3] = '\0';  
        for (ii = 0; ii < rows; ii++)  
        {  
            if (ii == 0) sprintf(msg, "%s = ", name);  
            else sprintf(msg, "%s", blanks);  
            if (cols > 0) ECHO(msg);  
            for (jj = 0; jj < cols; jj++)  
            {  
                sprintf(msg, "%d", array[kk++]);  
                ECHO(msg);  
                if (jj < cols - 1)  
                {  
                    sprintf(msg, ", ");  
                    ECHO(msg);  
                }  
            }  
            if (cols > 0) ECHO("\n");  
        }  
    }  
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        sprintf(msg, "%s = \"%s\"\n", title, string);  
        ECHO(msg);  
    }  
    static void display_temporary_text(char *text, double loc[3])  
    {  
        UF_OBJ_disp_props_t  
            props = { 1, 0, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        UF_CALL(UF_DISP_display_temporary_text(NULL_TAG, UF_DISP_USE_WORK_VIEW,  
            text, loc, UF_DISP_MIDDLECENTER, &props, 0, 0));  
    }  
    static void number_objects_with_tags(tag_t *objects, int n_objects)  
    {  
        logical  
            is_displayable;  
        int  
            ii;  
        double  
            loc[3];  
        char  
            msg[133];  
        for (ii = 0; ii < n_objects; ii++)  
        {  
            UF_CALL(UF_OBJ_is_displayable(objects[ii], &is_displayable));  
            if (!is_displayable) continue;  
            sprintf(msg, "%d - %d", ii+1, objects[ii]);  
            if (UF_OBJ_ask_name_origin(objects[ii], loc))  
            {  
                UF_CALL(UF_OBJ_set_name(objects[ii], "X"));  
                UF_CALL(UF_OBJ_ask_name_origin(objects[ii], loc));  
                UF_CALL(UF_OBJ_delete_name(objects[ii]));  
            }  
            display_temporary_text(msg, loc);  
        }  
    }  
    static void report_feature_name_type_and_sysname(tag_t feature)  
    {  
        char  
            *feature_name,  
            *feature_sysname,  
            *feature_type;  
        UF_CALL(UF_MODL_ask_feat_name( feature, &feature_name ));  
        WRITE_S(feature_name);  
        UF_free(feature_name);  
        UF_CALL(UF_MODL_ask_feat_sysname( feature, &feature_sysname ));  
        WRITE_S(feature_sysname);  
        UF_free(feature_sysname);  
        UF_CALL(UF_MODL_ask_feat_type( feature, &feature_type ));  
        WRITE_S(feature_type);  
        UF_free(feature_type);  
    }  
    static void do_it(void)  
    {  
        int  
            ii;  
        tag_t  
            feat;  
        char  
            *feature_type;  
        UF_MODL_reblend_face_data_t  
            reblend_data;  
        while ((feat = select_a_feature("Select Reblend Face")) != NULL_TAG)  
        {  
            report_feature_name_type_and_sysname(feat);  
            UF_CALL(UF_MODL_ask_feat_type(feat, &feature_type ));  
            if (strcmp(feature_type, "REBLEND_FACE") ||  
                UF_CALL(UF_MODL_ask_reblend_face(feat, &reblend_data)))  
                continue;  
            WRITE_D(reblend_data.num_chains);  
            WRITE_S(reblend_data.radius);  
            for (ii = 0; ii < reblend_data.num_chains; ii++)  
            {  
                WRITE_L(reblend_data.chain_data[ii].chained);  
                WRITE_D(reblend_data.chain_data[ii].n_faces);  
                WRITEnT(reblend_data.chain_data[ii].n_faces,  
                    reblend_data.chain_data[ii].faces_eid);  
                number_objects_with_tags(reblend_data.chain_data[ii].faces_eid,  
                    reblend_data.chain_data[ii].n_faces);  
                if (reblend_data.chain_data[ii].n_faces > 0)  
                    UF_free(reblend_data.chain_data[ii].faces_eid);  
            }  
            if (reblend_data.num_chains > 0) UF_free(reblend_data.chain_data);  
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

> 这段NX Open C++代码用于查询和显示NX中重铸面特征的数据。
>
> 主要功能包括：
>
> 1. 通过UF_UI_select_feature函数，让用户选择一个特征，然后循环处理用户选择的所有特征。
> 2. 通过UF_MODL_ask_feat_name、UF_MODL_ask_feat_sysname、UF_MODL_ask_feat_type函数获取特征名称、系统名称和类型，并显示。
> 3. 如果特征是重铸面特征，则使用UF_MODL_ask_reblend_face函数获取其数据，包括链的数量、半径、每条链是否关联以及链上的面等。
> 4. 遍历每条链，显示其是否关联、面的数量、面的ID数组，并在图形区显示面的ID编号。
> 5. 使用UF_DISP_display_temporary_text函数在图形区显示文字信息，用于显示面的ID编号。
> 6. 使用宏定义简化了错误处理和日志打印。
> 7. 包含了UF_initialize和UF_terminate函数，用于初始化和终止NX Open。
> 8. 实现了ufusr和ufusr_ask_unload函数，使代码可以编译为NX的自定义命令。
>
> 总体来说，这段代码实现了查询和显示NX中重铸面特征数据的功能，具有完善的错误处理和日志打印机制，是一个典型的NX Open二次开发示例。
>

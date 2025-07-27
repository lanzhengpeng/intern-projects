### 【2254】report ocf data and create new ocf feature 报告OCF数据并创建新的OCF特征

#### 代码

```cpp
    /*HEAD REPORT_OCF_DATA_AND_CREATE_NEW_OCF_FEATURE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_obj.h>  
    #include <uf_modl.h>  
    #include <uf_curve.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，V18版本中新增了一个函数UF_print_syslog。该函数用于打印系统日志。 */  
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
    #define WRITE_F(X) (write_double_to_listing_window(#X, X))  
    /* qq3123197280 */  
    static void write_double_to_listing_window(char *title, double n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %f\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
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
    static void do_it(void)  
    {  
        int  
            inx = 0;  
        tag_t   
            feature = NULL_TAG,  
            new_feature = NULL_TAG;  
         UF_CURVE_ocf_data_t   
            *offset_data;  
        char  
            msg[MAX_LINE_LENGTH + 1] = { "" };  
        if((feature = select_a_feature("Select OffsetCurve Feature:"))!=NULL_TAG)  
        {  
            UF_CALL( UF_CURVE_ask_ocf_data( feature, &offset_data ));  
            /* string data 里海译:string data的译文是字符串数据。 */  
            WRITE_D( offset_data->string_data->string_tag );  
            WRITE_D( offset_data->string_data->offset_direction );  
            WRITE_D( offset_data->string_data->num_offsets );  
            WRITE_D( offset_data->num_string_data );  
            for( inx = 0; inx < offset_data->num_string_data; inx++ )  
            {  
               WRITE_S( offset_data->string_data->offset_distances[inx].string );  
            }  
            WRITE( "\n" );  
            /* face data 里海译:面数据 */  
            WRITE_F( offset_data->dist_tol );  
            WRITE_F( offset_data->ang_tol );  
            WRITE_F( offset_data->string_tol );  
            WRITE_D( offset_data->face_data->face_tag );  
            WRITE( "\n" );  
            /* cross_boundary_mode data 里海译:跨边界模式数据 */  
            WRITE_D( offset_data->cross_boundary_mode );  
            switch ( offset_data->offset_method )  
            {  
                case  UF_CURVE_OCF_CHORDAL:  
                    sprintf( msg, "offset_data->offset_method=UF_CURVE_OCF_CHORDAL\n\n");  
                    break;  
                case  UF_CURVE_OCF_ARCLENGTH:  
                    sprintf( msg, "offset_data->offset_method=UF_CURVE_OCF_ARCLENGTH\n\n");  
                    break;  
                case UF_CURVE_OCF_GEODESIC:   
                    sprintf( msg, "offset_data->offset_method=UF_CURVE_OCF_GEODESIC\n\n");  
                    break;  
            }    
            WRITE( msg );  
            /* trim_method data 里海译:对不起，我无法完成这个任务。 */  
            switch ( offset_data->trim_method )  
            {  
                case  UF_CURVE_OCF_NO_EXTENSION:  
                    sprintf( msg, "trim_data->trim_method=UF_CURVE_OCF_NO_EXTENSION\n\n");  
                    break;  
                case  UF_CURVE_OCF_TANGENT:  
                    sprintf( msg, "trim_data->trim_method=UF_CURVE_OCF_TANGENT\n\n");  
                    break;  
            }    
            WRITE( msg );  
            switch ( offset_data->span_method )  
            {  
                case  UF_CURVE_OCF_SPAN_NONE:  
                    sprintf( msg, "span_data->span_method=UF_CURVE_OCF_SPAN_NONE\n\n");  
                    break;  
                case  UF_CURVE_OCF_SPAN_QUILT:  
                    sprintf( msg, "span_data->span_method=UF_CURVE_OCF_SPAN_QUILT\n\n");  
                    break;  
            }    
            WRITE( msg );  
        }  
        /* make changes to offset_data here to create a different OCF feature 里海译:修改offset_data中的内容，以创建不同的OCF特性。 */  
        strcpy( offset_data->string_data->offset_distances[0].string, ".250" );  
        UF_CALL( UF_CURVE_create_ocf_feature ( offset_data, &new_feature ));  
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

> 这是段NX二次开发代码，主要用于报告OCF(Offset Curve Feature)的数据并创建一个新的OCF特征。代码的主要功能和流程如下：
>
> 1. 包含必要的NX开发库头文件，并定义了错误报告函数report_error。
> 2. 定义了一些宏，用于将数据写入列表窗口。
> 3. 定义了函数select_a_feature，用于选择一个OCF特征。
> 4. 定义了函数do_it，用于执行以下流程：a. 调用select_a_feature函数选择一个OCF特征。b. 调用UF_CURVE_ask_ocf_data函数获取该特征的OCF数据。c. 将OCF数据的各个字段写入列表窗口，包括字符串数据、面数据、跨边界模式数据等。d. 对OCF数据进行修改，例如修改偏移距离。e. 调用UF_CURVE_create_ocf_feature函数，使用修改后的OCF数据创建一个新的OCF特征。
> 5. 定义了ufusr函数，作为NX的UF插件接口函数，在NX启动时调用do_it函数。
> 6. 定义了ufusr_ask_unload函数，返回立即卸载标志。
>
> 总的来说，这段代码实现了获取和修改OCF特征数据，并创建新的OCF特征的功能。代码结构清晰，流程明确，可作为二次开发的参考。
>

### 【1819】read csv text file and create points Read CSV text file and create points.

#### 代码

```cpp
    /*HEAD READ_CSV_TEXT_FILE_AND_CREATE_POINTS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <stdlib.h>  
    #include <uf.h>  
    #include <uf_obj.h>  
    #include <uf_modl.h>  
    #include <uf_part.h>  
    #include <uf_view.h>  
    #include <uf_curve.h>  
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
    static logical prompt_for_csv_text_name(char *prompt, char *fspec)  
    {  
        int  
            resp;  
        char  
            filter[MAX_FSPEC_SIZE+1];  
        strcpy(filter, "*.csv");  
        if (!UF_CALL(UF_UI_create_filebox(prompt, "CSV Text File Name", filter, "",  
            fspec, &resp)) && (resp != UF_UI_CANCEL)) return TRUE;  
        else return FALSE;  
    }  
    static void do_it(void)  
    {  
        int  
            inx = 0,  
            cntr = 1,  
            n_lines = 0;  
        tag_t  
            point = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        double  
             point_location[3] = { 0., 0., 0. };  
        char  
            msg[300] = { "" },  
            *txtptr = NULL,  
            pname[30] = { "" },  
            a_line[MAX_LINE_SIZE+1] = { "" },  
            fname[MAX_FSPEC_SIZE + 1] = { "" };  
        FILE  
            *csv_file = NULL;  
        if( NULL_TAG == part )  
        {  
            sprintf( msg, "No Current Part.  Program needs a part.\n\n" );  
            WRITE( msg );  
            return;  
        }  
        if(prompt_for_csv_text_name("Select CSV Text File:", fname))  
        {  
            csv_file = fopen( fname, "r" );  
            if( csv_file == NULL )  
            {  
                sprintf( msg, "Could not open the file, %s\n", fname);  
                WRITE( msg );  
                return;  
            }  
            else  
            {  
                while ((fgets(a_line, MAX_LINE_SIZE, csv_file)) != NULL) n_lines++;  
                rewind(csv_file);  
                for (inx = 0; inx < n_lines; inx++)  
                {  
                    fgets(a_line, MAX_LINE_SIZE, csv_file);  
                    if( (txtptr = strchr(a_line, ',')) == NULL ) continue;  
                    point_location[0] = atof( a_line );  
                    txtptr += sizeof( char );  
                    point_location[1] = atof( txtptr );  
                    txtptr = strchr(txtptr, ',' ) + sizeof( char );  
                    point_location[2] = atof( txtptr );  
                    UF_CALL( UF_CURVE_create_point( point_location, &point ));  
                    sprintf( pname, "POINT_%d", cntr++ );  
                    /* comment next line to avoid naming the points. 里海译:不要命名这些点，请翻译下一行。 */  
                    UF_CALL( UF_OBJ_set_name( point, pname ));  
                }  
                fclose( csv_file );  
                UF_VIEW_fit_view( NULL_TAG, 0.9 );  
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

> 这段代码是一个NX二次开发示例，主要功能是从CSV文件中读取点坐标并创建点对象。以下是代码的主要功能介绍：
>
> 1. 引入了必要的NX Open API头文件。
> 2. 定义了错误报告函数report_error，用于在函数调用出错时输出错误信息。
> 3. 定义了提示用户选择CSV文件名的函数prompt_for_csv_text_name。
> 4. 定义了主函数do_it，实现以下功能：获取当前显示的部件。提示用户选择CSV文件。打开CSV文件，并统计文件中的行数。逐行读取CSV文件，解析每行的x、y、z坐标。使用坐标创建点对象，并设置点的名称。在当前部件中创建所有点。关闭CSV文件。调整视图显示所有点。
> 5. 获取当前显示的部件。
> 6. 提示用户选择CSV文件。
> 7. 打开CSV文件，并统计文件中的行数。
> 8. 逐行读取CSV文件，解析每行的x、y、z坐标。
> 9. 使用坐标创建点对象，并设置点的名称。
> 10. 在当前部件中创建所有点。
> 11. 关闭CSV文件。
> 12. 调整视图显示所有点。
> 13. 定义了ufusr函数作为NX的启动函数，调用do_it函数。
> 14. 定义了ufusr_ask_unload函数，设置插件卸载模式为立即卸载。
>
> 总体来说，这段代码实现了从CSV文件中读取点坐标，并在NX中创建这些点对象的功能。
>

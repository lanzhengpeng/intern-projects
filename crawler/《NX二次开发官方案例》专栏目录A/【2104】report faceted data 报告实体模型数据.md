### 【2104】report faceted data 报告实体模型数据

#### 代码

```cpp
    /**  
     * report_faceted_data.c  
     */  
    /* Include files 里海译:Include files（包含文件）

Include files通常指的是在C语言或C++程序中用来包含其他源文件或头文件的语句，通过使用#include指令来实现。这种机制可以将一个文件的内容包含到另一个文件中，使得源代码可以重复使用，提高代码的复用性。同时，它还可以让源文件中的函数声明或宏定义等在多个源文件中共享。因此，include files在程序开发中是一种重要的代码组织方式。 */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_facet.h>  
    #include <uf_modl.h>  
    #include <uf_obj.h>  
    #include <uf_part.h>  
    #include <uf_std.h>  
    #include <uf_ui.h>  
    #include <uf_vec.h>  
    static logical bDebug = false;  
    static int nFacetBodySubType = UF_faceted_model_normal_subtype;  
    // Write outputs  
    void ECHO(char *format, ...)  
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
    // Error reporting  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    int report_error( char *file, int line, char *call, int irc)  
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
    // Cycle faceted bodies  
    tag_t ask_next_faceted_body(tag_t part, tag_t fbody)  
    {  
        int  
            subtype,  
            type;  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_faceted_model_type, &fbody))  
            && (fbody != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(fbody, &type, &subtype));  
            nFacetBodySubType = subtype;  
            return fbody;  
        }  
        return NULL_TAG;  
    }  
    void do_it()  
    {  
        tag_t  
            body = NULL_TAG,  
            part = NULL_TAG;  
        int  
            ii, jj,  
            num_facets = 0,  
            num_facets_cycle = 0,  
            num_vertices = 0,  
            facet_id = UF_FACET_NULL_FACET_ID;  
        double  
            plane_normal[3] = { 0.0, 0.0, 1.0 },  
            d_coefficient = 0.0,  
            (*vertices)[3];  
        double   
            value,  
            minmax[6] = { 10000, -10000, 10000, -10000, 10000, -10000 }; // xyz min/max  
        part = UF_PART_ask_display_part();  
        if (part == NULL_TAG)  
        {  
            ECHO("No displayed part...exit.\n");  
            return;  
        }  
        while ((body = ask_next_faceted_body(part, body)) != NULL_TAG)  
        {  
            // Count all facets in all faceted bodies  
            UF_CALL(UF_FACET_ask_n_facets_in_model(body, &num_facets));  
            ECHO("\nFaceted Body Tag %d is from type %d\n", body, nFacetBodySubType);  
            ECHO(" Facets: %d\n", num_facets);  
            // Query each faceted body for facets, normal and vertices  
            // reset values  
            num_facets_cycle = 0;  
            minmax[0] = 10000; minmax[1] = -10000;  
            minmax[2] = 10000; minmax[3] = -10000;  
            minmax[4] = 10000; minmax[5] = -10000;  
            UF_CALL(UF_FACET_cycle_facets(body, &facet_id));  
            while (facet_id != UF_FACET_NULL_FACET_ID)  
            {  
                num_facets_cycle++;  
                // UF_FACET_ask_plane_equation only works for NX Facet type  
                if (nFacetBodySubType == UF_faceted_model_normal_subtype)  
                {  
                    // obtain facet normal  
                    UF_CALL(UF_FACET_ask_plane_equation(body, facet_id, plane_normal, &d_coefficient));  
                    for (ii = 0; ii<3; ii++)  
                    {  
                        value = (float)plane_normal[ii];  
                        if (bDebug) ECHO("plane_normal[%d] = %f\n", ii, plane_normal[ii]);  
                    }  
                }  
                // obtain facet vertices in the current faceted body  
                UF_CALL(UF_FACET_ask_num_verts_in_facet(body, facet_id, &num_vertices));  
                vertices = (double(*)[3])malloc(3 * num_vertices*sizeof(double));  
                UF_CALL(UF_FACET_ask_vertices_of_facet(body, facet_id, &num_vertices, (double(*)[3])vertices));  
                // we should always have at least 3 vertices on each facet and never run into this error  
                if (num_vertices < 3)  
                {  
                    ECHO("ERROR: Facet has %d vertices only! Exit.\n", num_vertices);  
                    return;  
                }  
                // If Facet type is JT the calculate CrossProduct for plane normal now  
                if (nFacetBodySubType != UF_faceted_model_normal_subtype)  
                {  
                    double vec1[3] = { vertices[1][0] - vertices[0][0], vertices[1][1] - vertices[0][0], vertices[1][2] - vertices[0][0] };  
                    double vec2[3] = { vertices[2][0] - vertices[0][0], vertices[2][1] - vertices[0][0], vertices[2][2] - vertices[0][0] };  
                    UF_VEC3_cross(vec1, vec2, plane_normal);  
                    for (ii = 0; ii<3; ii++)  
                    {  
                        value = (float)plane_normal[ii];  
                        if (bDebug) ECHO("plane_normal[%d] = %f\n", ii, plane_normal[ii]);  
                    }  
                }  
                for (ii = 0; ii<num_vertices; ii++)  
                {  
                    for (jj = 0; jj<3; jj++)  
                    {  
                        float value = (float)vertices[ii][jj];  
                        if (bDebug) ECHO("vertice[%d][%d] = %f\n", ii, jj, vertices[ii][jj]);  
                        // get min max values  
                        if (value < minmax[jj * 2]) minmax[jj * 2] = value;  
                        if (value > minmax[jj * 2 + 1]) minmax[jj * 2 + 1] = value;  
                    }  
                }  
                if (vertices) free(vertices);  
                // Get next facet  
                UF_CALL(UF_FACET_cycle_facets(body, &facet_id));  
            }  
            ECHO(" Facets (cycle): %d\n", num_facets_cycle);  
            ECHO(" Xmin=%.3f Xmax=%.3f\n Ymin=%.3f Ymax=%.3f\n Zmin=%.3f Zmax=%.3f\n Xdelta=%.3f\n Ydelta=%.3f\n Zdelta=%.3f\n",  
                minmax[0], minmax[1], minmax[2], minmax[3], minmax[4], minmax[5],  
                minmax[1] - minmax[0], minmax[3] - minmax[2], minmax[5] - minmax[4]);  
        } // while  
    }  
    static void report_load_status(UF_PART_load_status_p_t status)  
    {  
        char  
            msg[133];  
        int  
            ii;  
        if (status->n_parts > 0)  
        {  
            UF_UI_open_listing_window();  
            UF_UI_write_listing_window("Load notes:\n");  
            for (ii = 0; ii < status->n_parts; ii++)  
            {  
                UF_UI_write_listing_window("  ");  
                UF_UI_write_listing_window(status->file_names[ii]);  
                UF_UI_write_listing_window(" - ");  
                UF_get_fail_message(status->statuses[ii], msg);  
                UF_UI_write_listing_window(msg);  
                UF_UI_write_listing_window("\n");  
            }  
            UF_UI_write_listing_window("\n");  
            UF_free(status->statuses);  
            UF_free_string_array(status->n_parts, status->file_names);  
        }  
    }  
    //  Internal Entry Point  
    extern DllExport void ufusr( char *parm, int *returnCode, int rlen )  
    {  
        if (!UF_CALL(UF_initialize()))  
        {  
            char   
                part_name[MAX_FSPEC_BUFSIZE] = "";  
            tag_t   
                part = NULL_TAG,  
                orig_part = UF_PART_ask_display_part();  
            int   
                response;  
            logical   
                unused = 0;  
            UF_PART_load_status_t  
                status;  
            if (orig_part == NULL_TAG &&  
                !(UF_CALL(UF_UI_ask_open_part_filename(part_name, &unused, &response))) &&  
                (response == UF_UI_OK))  
            {  
                UF_CALL(UF_PART_open(part_name, &part, &status));  
                report_load_status(&status);  
                if (!status.failed)  
                {  
                    do_it();  
                }  
            }  
            else  
                do_it();  
            UF_CALL(UF_terminate());  
        }  
    }  
    // Unload Handler  
    extern int ufusr_ask_unload( void )  
    {  
        return( UF_UNLOAD_IMMEDIATELY );  
    }  
    // External Entry Point  
    // Usage: program.exe /path/to/part1.prt /path/to/part2.jt [...]  
    int main(int argc, char *argv[])  
    {  
        int  
            cnt = 0;  
        tag_t  
            part;  
        char  
            part_name[MAX_FSPEC_SIZE + 1];  
        UF_PART_load_status_t  
            status;  
        if (!UF_CALL(UF_initialize()))  
        {  
            UF_CALL(uc4624(0, argc, argv));  
            while (uc4621(part_name) == 1)  
            {  
                ECHO("%d.  %s\n", ++cnt, part_name);  
                UF_CALL(UF_PART_open(part_name, &part, &status));  
                report_load_status(&status);  
                if (!status.failed)  
                {  
                    do_it();  
                }  
            }  
            ECHO("\nProcessed %d parts.\n", cnt);  
            UF_CALL(UF_terminate());  
        }  
        return 0;  
    }

```

#### 代码解析

> 这段代码是一个NX Open的二次开发示例，其主要功能是遍历当前显示部件中的所有Facet Body，并统计每个Facet Body中的Facet数量、顶点数量以及坐标范围。以下是代码的主要流程：
>
> 1. 包含必要的NX Open头文件。
> 2. 定义错误报告函数report_error()，用于在函数调用出错时报告错误信息。
> 3. 定义遍历Facet Body的函数ask_next_faceted_body()，用于获取当前部件中的下一个Facet Body。
> 4. 定义主要功能函数do_it()，用于遍历Facet Body并统计信息。
> 5. 在do_it()中，首先获取当前显示的部件，然后循环获取每个Facet Body。
> 6. 对于每个Facet Body，统计Facet数量，遍历每个Facet，获取法向量、顶点信息，并统计顶点坐标范围。
> 7. 输出每个Facet Body的Facet数量、顶点数量及坐标范围。
> 8. 在ufusr()函数中，如果当前没有打开部件，提示用户打开一个部件，然后调用do_it()。
> 9. 在main()函数中，遍历命令行传入的部件文件，打开并调用do_it()。
> 10. 定义卸载处理函数ufusr_ask_unload()，用于在插件卸载时调用。
>
> 总体来说，这段代码实现了对一个或多个NX部件的Facet Body的遍历统计，输出了有用的信息，具有一定的参考价值。
>

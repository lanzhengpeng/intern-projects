### 【0698】create facet model 创建面模型

#### 代码

```cpp
    /*****************************************************************************  
    **  
    ** create_facet_model.c  
    **  
    ** Description:  
    **     Contains Unigraphics entry points for the application.  
    **  
    *****************************************************************************/  
    /* Include files 译:Include files 的翻译是包含文件。 */  
    #include <stdarg.h>  
    #include <stdio.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_ui_types.h>  
    #include <uf_part.h>  
    #include <uf_facet.h>  
    static void ECHO(char *format, ...)  
    {  
        char msg[1025];  
        va_list args;  
        va_start(args, format);  
        vsnprintf_s(msg, sizeof(msg), 1024, format, args);  
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
    static void do_it(void)  
    {  
        tag_t        part_tag;  
        int          i;  
        tag_t        new_model;  
        double       facet_vertices[30][3];  
        double       facet_normals[30][3];  
        int          adjacencies[3];  
        int          facets[7];  
        int          edge;  
        int          adjacent_facets[3];  
        int          facet_id;  
        int          edge_in_adjacent_facet;  
        int          num_facets_in_model;  
        logical      model_convexity;  
        part_tag = UF_PART_ask_display_part();  
        if(part_tag == NULL_TAG)  
        {  
            ECHO("No displayed part. Exit.\n");  
            return;  
        }  
        UF_CALL(UF_FACET_create_model( part_tag, &new_model ));  
        facet_vertices[0][0] = 0.0;  
        facet_vertices[0][1] = 0.0;  
        facet_vertices[0][2] = 0.0;  
        facet_vertices[1][0] = 0.03;  
        facet_vertices[1][1] = 0.05;  
        facet_vertices[1][2] = 0.0;  
        facet_vertices[2][0] = 0.060;  
        facet_vertices[2][1] = 0.0;  
        facet_vertices[2][2] = 0.0;  
        adjacencies[0] = UF_FACET_NULL_FACET_ID;  
        adjacencies[1] = UF_FACET_NULL_FACET_ID;  
        adjacencies[2] = UF_FACET_NULL_FACET_ID;  
        UF_CALL( UF_FACET_add_facet_to_model( new_model,  
                                    3,  
                                    facet_vertices,  
                                    NULL,  
                                    adjacencies,  
                                    &facets[0] ));  
        facet_vertices[0][0] = 0.0;  
        facet_vertices[0][1] = 0.0;  
        facet_vertices[0][2] = 0.0;  
        facet_vertices[1][0] = 0.060;  
        facet_vertices[1][1] = 0.0;  
        facet_vertices[1][2] = 0.0;  
        facet_vertices[2][0] = 0.03;  
        facet_vertices[2][1] = 0.03;  
        facet_vertices[2][2] = 0.05;  
        adjacencies[0] = facets[0];  
        UF_CALL( UF_FACET_add_facet_to_model( new_model,  
                                    3,  
                                    facet_vertices,  
                                    NULL,  
                                    adjacencies,  
                                    &facets[1] ));  
        facet_vertices[0][0] = 0.060;  
        facet_vertices[0][1] = 0.0;  
        facet_vertices[0][2] = 0.0;  
        facet_vertices[1][0] = 0.03;  
        facet_vertices[1][1] = 0.05;  
        facet_vertices[1][2] = 0.0;  
        facet_vertices[2][0] = 0.03;  
        facet_vertices[2][1] = 0.03;  
        facet_vertices[2][2] = 0.05;  
        adjacencies[0] = facets[0];  
        adjacencies[1] = UF_FACET_NULL_FACET_ID;  
        adjacencies[2] = facets[1];  
        UF_CALL( UF_FACET_add_facet_to_model( new_model,  
                                    3,  
                                    facet_vertices,  
                                    NULL,  
                                    adjacencies,  
                                    &facets[2] ));  
        facet_vertices[0][0] = 0.03;  
        facet_vertices[0][1] = 0.05;  
        facet_vertices[0][2] = 0.0;  
        facet_vertices[1][0] = 0.0;  
        facet_vertices[1][1] = 0.0;  
        facet_vertices[1][2] = 0.0;  
        facet_vertices[2][0] = 0.03;  
        facet_vertices[2][1] = 0.03;  
        facet_vertices[2][2] = 0.05;  
        adjacencies[0] = facets[0];  
        adjacencies[1] = facets[1];  
        adjacencies[2] = facets[2];  
        UF_CALL(UF_FACET_add_facet_to_model( new_model,  
                                    3,  
                                    facet_vertices,  
                                    NULL,  
                                    adjacencies,  
                                    &facets[3] ));  
        facet_id = UF_FACET_NULL_FACET_ID;  
        UF_CALL(UF_FACET_cycle_facets( new_model, &facet_id ));  
        while ( facet_id != UF_FACET_NULL_FACET_ID )  
        {  
            for ( edge = 0; edge < 3; edge++ )  
                UF_FACET_ask_adjacent_facet( new_model,  
                                            facet_id,  
                                            edge,  
                                            &adjacent_facets[edge],  
                                            &edge_in_adjacent_facet );  
            ECHO( "facet index: %d\n", facet_id );  
            ECHO( "\tadjacent_facets: [%d,%d,%d]\n",  
                   adjacent_facets[0],  
                   adjacent_facets[1],  
                   adjacent_facets[2] );  
            UF_CALL(UF_FACET_cycle_facets( new_model, &facet_id ));  
        }  
        UF_CALL(UF_FACET_is_model_convex( new_model, &model_convexity ));  
        ECHO( "Model %s convex\n",  
               (model_convexity) ? "IS" : "IS NOT" );  
        /*  
           Now create a facet topology containing some concave  
           edges, to get this delete the first facet in the tetrahedron  
           and replace it with three facets the shared vertex of which  
           is towards the top vertex of the tetrahedron.  
        */  
        UF_CALL(UF_FACET_del_facet_from_model( new_model, facets[0] ));  
        facet_vertices[0][0] = 0.060;  
        facet_vertices[0][1] = 0.0;  
        facet_vertices[0][2] = 0.0;  
        facet_vertices[1][0] = 0.0;  
        facet_vertices[1][1] = 0.0;  
        facet_vertices[1][2] = 0.0;  
        facet_vertices[2][0] = 0.02;  
        facet_vertices[2][1] = 0.02;  
        facet_vertices[2][2] = 0.01;  
        adjacencies[0] = facets[1];  
        adjacencies[1] = UF_FACET_NULL_FACET_ID;  
        adjacencies[2] = UF_FACET_NULL_FACET_ID;  
        UF_CALL(UF_FACET_add_facet_to_model( new_model,  
                                    3,  
                                    facet_vertices,  
                                    NULL,  
                                    adjacencies,  
                                    &facets[4] ));  
        facet_vertices[0][0] = 0.03;  
        facet_vertices[0][1] = 0.05;  
        facet_vertices[0][2] = 0.0;  
        facet_vertices[1][0] = 0.060;  
        facet_vertices[1][1] = 0.0;  
        facet_vertices[1][2] = 0.0;  
        facet_vertices[2][0] = 0.02;  
        facet_vertices[2][1] = 0.02;  
        facet_vertices[2][2] = 0.01;  
        adjacencies[0] = facets[2];  
        adjacencies[1] = facets[4];  
        adjacencies[2] = UF_FACET_NULL_FACET_ID;  
        UF_CALL(UF_FACET_add_facet_to_model( new_model,  
                                    3,  
                                    facet_vertices,  
                                    NULL,  
                                    adjacencies,  
                                    &facets[5] ));  
        facet_vertices[0][0] = 0.0;  
        facet_vertices[0][1] = 0.0;  
        facet_vertices[0][2] = 0.0;  
        facet_vertices[1][0] = 0.03;  
        facet_vertices[1][1] = 0.05;  
        facet_vertices[1][2] = 0.0;  
        facet_vertices[2][0] = 0.02;  
        facet_vertices[2][1] = 0.02;  
        facet_vertices[2][2] = 0.01;  
        adjacencies[0] = facets[3];  
        adjacencies[1] = facets[5];  
        adjacencies[2] = facets[4];  
        UF_CALL(UF_FACET_add_facet_to_model( new_model,  
                                    3,  
                                    facet_vertices,  
                                    NULL,  
                                    adjacencies,  
                                    &facets[6] ));  
        UF_CALL(UF_FACET_model_edits_done( new_model ));  
        // Now look at the edge convexity again.   
        ECHO( "Edge convexity for %u\n", new_model );  
        facet_id = UF_FACET_NULL_FACET_ID;  
        UF_FACET_cycle_facets( new_model, &facet_id );  
        while ( facet_id != UF_FACET_NULL_FACET_ID )  
        {  
            int verts_in_facet;  
            UF_CALL( UF_FACET_ask_num_verts_in_facet( new_model,  
                                            facet_id,  
                                            &verts_in_facet ));  
            // For each vertex print the vertex coordinates and the  vertex normal.  
            UF_CALL(UF_FACET_ask_vertices_of_facet( new_model,  
                                           facet_id,  
                                           &verts_in_facet,  
                                           facet_vertices ));  
            UF_CALL(UF_FACET_ask_normals_of_facet( new_model,  
                                          facet_id,  
                                          &verts_in_facet,  
                                          facet_normals ));  
            for ( i=0 ; i < verts_in_facet ; i++ )  
            {  
                ECHO( "  Vertex %d: (%g, %g, %g)\n",  
                       i, facet_vertices[i][0],  
                          facet_vertices[i][1],  
                          facet_vertices[i][2] );  
                ECHO( "  Normal %d (%g, %g, %g)\n",  
                       i, facet_normals[i][0],  
                          facet_normals[i][1],  
                          facet_normals[i][2] );  
            }  
            for ( i=0 ; i < verts_in_facet ; i++ )  
            {  
                int convexity;  
                UF_CALL(UF_FACET_ask_edge_convexity( new_model,  
                                            facet_id,  
                                            i,  
                                            &convexity ));  
                ECHO( "  Facet %d: Edge %d: %s\n", facet_id, i,  
                        (convexity == UF_FACET_IS_CONVEX)  ?  
                                "IS CONVEX" :  
                        (convexity == UF_FACET_IS_CONCAVE) ?  
                                "IS CONCAVE" :  
                                "CONVEXITY IS NOT DETERMINED"  
                      );  
                //   Now check that the convexity of the corresponding  edge in the adjacent facet is consistent.  
                {  
                    int adjacent_facet_id;  
                    int adj_convexity;  
                    UF_CALL( UF_FACET_ask_adjacent_facet( new_model,  
                                                facet_id,  
                                                i,  
                                                &adjacent_facet_id,  
                                                &edge_in_adjacent_facet  
                                               ));  
                    UF_CALL(UF_FACET_ask_edge_convexity( new_model,  
                                                adjacent_facet_id,  
                                                edge_in_adjacent_facet,  
                                                &adj_convexity ));  
                    if ( convexity != adj_convexity )  
                       ECHO( "  **ERR: Edge convexity wrong\n" );  
                }  
            }  
            UF_CALL(UF_FACET_cycle_facets( new_model, &facet_id ));  
        }  
        UF_CALL(UF_FACET_ask_n_facets_in_model(new_model, &num_facets_in_model));  
        ECHO( "There are %d facets in the final model\n", num_facets_in_model );  
        UF_CALL(UF_FACET_is_model_convex( new_model, &model_convexity ));  
        ECHO( "Model %s convex\n", (model_convexity) ? "IS" : "IS NOT" );  
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
        if( UF_CALL(UF_initialize()) )   
        {  
            /* Failed to initialize 译:Failed to initialize 翻译为“初始化失败”。 */  
            return;  
        }  
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

> 这段代码是一个NX的二次开发示例，主要实现了以下功能：
>
> 1. 创建一个NX部件，并创建一个Facet模型。
> 2. 向Facet模型中添加多个Facet面，并设置相邻关系。
> 3. 遍历模型中的所有Facet面，并输出每个Facet面的顶点坐标、法向量，以及每条边的凸凹性。
> 4. 判断整个Facet模型的凸凹性。
> 5. 删除模型中的一个Facet面，并添加新的Facet面，使模型变为凹模型。
> 6. 再次遍历模型中的所有Facet面，输出每个Facet面的顶点坐标、法向量，以及每条边的凸凹性。
> 7. 输出模型中Facet面的数量，并判断整个Facet模型的凸凹性。
>
> 这段代码通过NX提供的Facet API实现了Facet模型的创建和操作，展示了Facet模型在NX中的基本使用方法。
>

### 【2075】report drawing info 报告图纸信息

#### 代码

```cpp
    static tag_t report_drawing_info(tag_t drawing)  
    {  
        char  
            dname[MAX_ENTITY_NAME_SIZE+1];  
        UF_DRAW_info_t drawing_info;  
        UF_CALL(UF_OBJ_ask_name(drawing, dname));  
        UF_CALL(UF_DRAW_ask_drawing_info( drawing, &drawing_info));  
        printf("\t%s\n", dname);  
        printf( "The value of the drawing info structure is:\n" );  
        printf( "size_state:  %d\n", drawing_info.size_state );  
        if(drawing_info.size_state == UF_DRAW_METRIC_SIZE )  
                    printf( "standard metric size %d\n",  
                        drawing_info.size.metric_size_code );  
        else if(drawing_info.size_state == UF_DRAW_ENGLISH_SIZE )  
                    printf( "standard english size %d\n",  
                        drawing_info.size.english_size_code );  
        else  
          printf("custom size = %f,  %f\n",  
            drawing_info.size.custom_size[0],   
            drawing_info.size.custom_size[1] );  
        printf( "scale:  %f\n", drawing_info.drawing_scale );  
        if(drawing_info.projection_angle ==  
            UF_DRAW_THIRD_ANGLE_PROJECTION )  
                printf(" projection angle : third \n" );  
        else  
                printf(" projection angle : first \n" );  
            printf("========================================\n");  
    }

```

#### 代码解析

> 这段代码是NX二次开发代码，用于获取NX绘图的相关信息并打印出来。主要功能包括：
>
> 1. 获取绘图对象的名称。
> 2. 获取绘图的信息，包括绘图尺寸状态、标准尺寸、自定义尺寸、绘图比例尺、投影角度等。
> 3. 根据绘图尺寸状态，打印标准尺寸或自定义尺寸。
> 4. 打印绘图比例尺。
> 5. 判断投影角度是第三视角还是第一视角，并打印出来。
> 6. 在打印时，对绘图对象名称进行了打印，并对不同绘图信息进行了格式化打印。
>
> 总的来说，这段代码实现了对NX绘图信息的获取和打印，便于开发人员查看和理解NX绘图的相关设置。
>

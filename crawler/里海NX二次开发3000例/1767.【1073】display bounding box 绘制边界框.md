### 【1073】display bounding box 绘制边界框

#### 代码

```cpp
    static void display_bounding_box( double llc[3], double urc[3])  
    {  
        double  
            box[6],  
            end[3],  
            start[3];  
        UF_OBJ_disp_props_t  
            props = { 1, UF_OBJ_WHITE, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        UF_VEC3_copy(llc, box);  
        UF_VEC3_copy(urc, &(box[3]));  
        start[0] = box[0];  
        start[1] = box[1];  
        start[2] = box[2];  
        end[0] = box[3];  
        end[1] = box[1];  
        end[2] = box[2];  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_WORK_VIEW,  
            start, end, &props));  
        start[0] = box[3];  
        start[1] = box[1];  
        start[2] = box[2];  
        end[0] = box[3];  
        end[1] = box[1];  
        end[2] = box[5];  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_WORK_VIEW,  
            start, end, &props));  
        start[0] = box[3];  
        start[1] = box[1];  
        start[2] = box[5];  
        end[0] = box[0];  
        end[1] = box[1];  
        end[2] = box[5];  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_WORK_VIEW,  
            start, end, &props));  
        start[0] = box[0];  
        start[1] = box[1];  
        start[2] = box[5];  
        end[0] = box[0];  
        end[1] = box[1];  
        end[2] = box[2];  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_WORK_VIEW,  
            start, end, &props));  
        start[0] = box[0];  
        start[1] = box[1];  
        start[2] = box[2];  
        end[0] = box[0];  
        end[1] = box[4];  
        end[2] = box[2];  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_WORK_VIEW,  
            start, end, &props));  
        start[0] = box[3];  
        start[1] = box[1];  
        start[2] = box[2];  
        end[0] = box[3];  
        end[1] = box[4];  
        end[2] = box[2];  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_WORK_VIEW,  
            start, end, &props));  
        start[0] = box[3];  
        start[1] = box[1];  
        start[2] = box[5];  
        end[0] = box[3];  
        end[1] = box[4];  
        end[2] = box[5];  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_WORK_VIEW,  
            start, end, &props));  
        start[0] = box[0];  
        start[1] = box[1];  
        start[2] = box[5];  
        end[0] = box[0];  
        end[1] = box[4];  
        end[2] = box[5];  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_WORK_VIEW,  
            start, end, &props));  
        start[0] = box[0];  
        start[1] = box[4];  
        start[2] = box[2];  
        end[0] = box[3];  
        end[1] = box[4];  
        end[2] = box[2];  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_WORK_VIEW,  
            start, end, &props));  
        start[0] = box[3];  
        start[1] = box[4];  
        start[2] = box[2];  
        end[0] = box[3];  
        end[1] = box[4];  
        end[2] = box[5];  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_WORK_VIEW,  
            start, end, &props));  
        start[0] = box[3];  
        start[1] = box[4];  
        start[2] = box[5];  
        end[0] = box[0];  
        end[1] = box[4];  
        end[2] = box[5];  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_WORK_VIEW,  
            start, end, &props));  
        start[0] = box[0];  
        start[1] = box[4];  
        start[2] = box[5];  
        end[0] = box[0];  
        end[1] = box[4];  
        end[2] = box[2];  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_WORK_VIEW,  
            start, end, &props));  
    }

```

#### 代码解析

> 这段代码是一个用于在NX中显示边界框的函数。主要功能包括：
>
> 1. 定义边界框的8个顶点，通过复制下边界左角和上边界右角的坐标到数组box中。
> 2. 绘制边界框的12条边，通过循环调用UF_DISP_display_temporary_line函数，在NX的工作视图中绘制线条。
> 3. 设置线条的颜色、线宽、字体等属性，通过UF_DISP_display_temporary_line函数的props参数。
> 4. 处理边界框的顶点坐标，通过复制和赋值操作来处理起始点和结束点的坐标。
> 5. 调用UF_DISP_display_temporary_line函数，实现线条的绘制。
> 6. 采用循环结构，通过循环绘制边界框的每条边。
> 7. 错误处理，通过UF_CALL宏来处理函数调用错误。
>
> 综上所述，该代码主要用于在NX中绘制一个三维边界框，通过循环绘制线条的方式实现，并设置了线条的颜色、线宽等属性。
>

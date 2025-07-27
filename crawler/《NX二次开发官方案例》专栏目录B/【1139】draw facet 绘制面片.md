### 【1139】draw facet 绘制面片

#### 代码

```cpp
    static void draw_facet(tag_t model, int facet)  
    {  
        int  
            ii,  
            n;  
        tag_t  
            line_tag;  
        double  
            verts[5][3];  
        UF_CURVE_line_t  
            line_data;  
        UF_CALL(UF_FACET_ask_vertices_of_facet(model, facet, &n, verts));  
        for (ii = 0; ii < (n - 1); ii++)  
        {  
            UF_VEC3_copy(&verts[ii][0], line_data.start_point);  
            UF_VEC3_copy(&verts[ii+1][0], line_data.end_point);  
            UF_CALL(UF_CURVE_create_line(&line_data, &line_tag));  
        }  
        UF_VEC3_copy(&verts[n-1][0], line_data.start_point);  
        UF_VEC3_copy(&verts[0][0], line_data.end_point);  
        UF_CALL(UF_CURVE_create_line(&line_data, &line_tag));  
    }

```

#### 代码解析

> 这是段NX Open C++的二次开发代码，用于绘制NX模型的一个面片(facet)的边界线。代码的主要步骤如下：
>
> 1. 定义一个静态方法draw_facet，接受两个参数：模型tag和面片索引。
> 2. 调用UF_FACET_ask_vertices_of_facet函数，获取面片的顶点坐标，存储在verts数组中。
> 3. 遍历顶点数组，每次取两个顶点，调用UF_CURVE_create_line创建一条直线。
> 4. 最后，闭合边界，将最后一个顶点和第一个顶点也连接起来。
> 5. 调用UF_CURVE_create_line创建闭合的边界线。
> 6. 使用UF_VEC3_copy进行向量赋值。
> 7. UF_CALL宏用于调用UF函数，并进行错误检查。
>
> 通过调用这个方法，可以绘制出NX模型指定面片的边界线。
>

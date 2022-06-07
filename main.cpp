///////////////////////////////////////////////////////////////////////////////
//5.
///////////////////////////////////////////////////////////////////////////////
//Ќаписать программу дл€ решени€ задачи коммиво€жЄра с помощью метода ветвей и границ.
//»нтерфейс должен позвол€ть вводить количество городов (вершин графа) и значени€
//элементов матрицы рассто€ний между городами (матрицы смежности).
///////////////////////////////////////////////////////////////////////////////
#include <algorithm>
#include <iostream>
#include <limits>
#include <set>
#include <vector>
#include <fstream>
#include <string>
#include <cmath>

using namespace std;
///////////////////////////////////////////////////////////////////////////////
typedef size_t                          T_town;
typedef std::vector     < T_town    >   T_path_val;
typedef float                           T_dist;
typedef std::vector     < T_dist    >   T_row;
typedef std::vector     < T_row     >   T_matr;
typedef std::set        < T_town    >   T_towns;
///////////////////////////////////////////////////////////////////////////////
class   T_path
{
    //-------------------------------------------------------------------------
    T_matr      dist_matr_;

    T_path_val  path_cur_;
    T_dist      path_cur_len_;

    T_path_val  shortest_path_;
    T_dist      shortest_path_len_;

    T_towns     free_towns_;
    //-------------------------------------------------------------------------
public:
    //-------------------------------------------------------------------------
    T_path( T_matr  const   &   dist_matr )
        :
        dist_matr_          ( dist_matr ),

        shortest_path_len_  (
                                std::numeric_limits< T_dist >::max()
                            )
    {
        for( T_town  t{}; t < dist_matr.size(); ++t )
        {
            free_towns_.insert(t);
        }

        push_town_with_delta_dist(0);
    }
    //-------------------------------------------------------------------------
    void    find_and_print_shortest()
    {
        find_shortest   ();
        //print_shortest  ();
    }
    //-------------------------------------------------------------------------
private:
    //-------------------------------------------------------------------------
    void    push_town_with_delta_dist
        (
            T_town  town,
            T_dist  delta_dist  =   0
        )
    {
        path_cur_   .emplace_back   ( town );
        free_towns_ .erase          ( town );
        path_cur_len_   +=  delta_dist;
    }
    //-------------------------------------------------------------------------
    void    find_shortest()
    {
        do
        {
            if  (
                    successfully_fill_better_path()
                )
            {
                shortest_path_      =   path_cur_;
                shortest_path_len_  =   path_cur_len_;
            }//if
        }
        while   (
                    successfully_inc_back_town()
                );
    }
    //-------------------------------------------------------------------------
    bool    successfully_fill_better_path()
    {
        while   (
                        !path_is_full                       ()
                    &&  successfully_push_min_good_town     ()
                );

        return  path_is_full();
    }
    //-------------------------------------------------------------------------
    bool    path_is_full()
    {
        return      path_cur_   .size()
                >=  dist_matr_  .size() + 1;
    }
    //-------------------------------------------------------------------------
    bool    successfully_push_min_good_town()
    {
        return      path_cur_   .size()
                ==  dist_matr_  .size()
                    ?   successfully_push_good_town                         (0)
                    :   successfully_push_good_min_free_town_not_less_than  (0);
    }
    //-------------------------------------------------------------------------
    bool    successfully_push_good_town( T_town     town )
    {
        auto    delta_dist  =   dist_matr_  [ path_cur_.back()  ]
                                            [ town              ];


        bool    bool_res    =           path_cur_len_
                                    +   delta_dist

                                <   shortest_path_len_;

        if( bool_res )
        {
            push_town_with_delta_dist
                (
                    town,
                    delta_dist
                );
        }

        return  bool_res;
    }
    //-------------------------------------------------------------------------
    bool    successfully_push_good_min_free_town_not_less_than( T_town  town_start )
    {
        return      std::find_if
                        (
                            free_towns_.lower_bound     ( town_start ),
                            free_towns_.end             (),


                            [&]                         ( auto  town )
                            {
                                return  this->successfully_push_good_town( town );
                            }
                        )

                !=  free_towns_.end();
    }
    //-------------------------------------------------------------------------
    bool    successfully_inc_back_town()
    {
        return          path_cur_.size()
                    >   1

                &&  (
                            successfully_push_good_min_free_town_not_less_than
                                (
                                    pop_and_get_town() + 1
                                )

                        ||  successfully_inc_back_town()
                    );
    }
    //-------------------------------------------------------------------------
    T_town  pop_and_get_town()
    {
        auto    back_town           =   path_cur_.back();
        path_cur_.pop_back();

        if( back_town )
        {
            free_towns_.insert( back_town );
        }

        auto    penultimate_town    =   path_cur_.back();

        path_cur_len_               -=  dist_matr_  [ penultimate_town  ]
                                                    [ back_town         ];

        return  back_town;
    }
    //-------------------------------------------------------------------------
    void    print_shortest()
    {



        std::cout   << shortest_path_len_ << std::endl;

        for( auto   town    :   shortest_path_ )
        {
            std::cout   <<  town + 1
                        <<  '\t';
        }//for

        std::cout   <<  std::endl;
    }
public:
    T_path_val shortest_path_towns()
    {
        return shortest_path_;
    }



    //-------------------------------------------------------------------------
};
///////////////////////////////////////////////////////////////////////////////
float town_distance(float x1, float y1, float x2, float y2)
{
    float dist = pow((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2), 0.5);
    return dist;

}

///////////////////////////////////////////////////////////////////////////////
int     main()
{
    std::ios::sync_with_stdio(false);
    int     towns_total{};

    // read from file
    string buf;
    string filename = "tsp_5_1";
    ifstream file("/Users/nikpav/Desktop/c++/Yandex/Yandex/fki/fki/fki 2kurs/TSP/tsp_data" + filename);
    getline(file, buf);
    towns_total = stoi(buf);
    //cout << towns_total << endl;

    vector<float> x(towns_total);
    vector<float> y(towns_total);


    int i_space;
    for (int i=0; i < towns_total; i++)
    {
        getline(file, buf);
        i_space = buf.find(' ');
        x[i] = stof(buf.substr(0, i_space));
        y[i] = stof(buf.substr(i_space+1));

        //cout << x[i] << " " << y[i] << endl;
    }
    //cout << endl;
    file.close();

    // find all distances between each of towns
    T_matr dist_matr(towns_total, T_row( towns_total ));

    for(int i=0; i<towns_total; i++)
    {
        for(int j=0; j<towns_total; j++)
        {
            if (i==j)
                dist_matr[i][j] = 0.0;
            else
                dist_matr[i][j] = town_distance(x[i],y[i],x[j],y[j]);
            //std::cout << dist_matr[i][j] << " ";
        }
        //std::cout << std::endl;
    }







    /*
    std::cout << "distance matrix:" << std::endl;
    for( T_town L{}; L < T_town( towns_total ); ++L )
        {
        for( T_town R{}; R < T_town( towns_total ); ++R )
            std::cout << dist_matr[L][R] << "\t";
        std::cout << std::endl;
        }
    */


    T_path  path                    ( dist_matr );
    path.find_and_print_shortest    ();

    T_path_val path_found = path.shortest_path_towns();
    float total_distance = 0.0;


    int k,j;
    for (int i=1; i<path_found.size(); i++)
    {
        k = path_found[i];
        j = path_found[i-1];
        total_distance += town_distance(x[j],y[j],x[k],y[k]);
       // cout << x[j] << " " << y[j] << " " << x[k] << " " << y[k] << endl;

        //cout << path_found[i] << '\t';
    }
    //cout << endl;


    cout << total_distance << endl;


    for( auto   town    :   path_found)
            std::cout   <<  town   <<  ' ';
    //cout << endl;
}

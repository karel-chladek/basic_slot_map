#include <iostream>
#include "slotmap.hpp"

#include "catch.hpp"

template struct slot_map< int >;

TEST_CASE( "constructors" ) {
    slot_map< int > sm;
    REQUIRE( sm.empty() );
    REQUIRE( sm.size() == 0 );

    slot_map< int > sm2( sm );
    REQUIRE( sm.empty() );
    REQUIRE( sm.size() == 0 );

    typename slot_map< int >::handle h;
}

TEST_CASE( "basic interface" ) {
    slot_map< int > sm;
    auto h0 = sm.insert( 0 );
    auto h1 = sm.insert( 1 );
    REQUIRE( sm.valid( h0 ) );
    sm.erase( h1 );
    sm[ h0 ] = 42;
    for ( auto &x : sm )
        REQUIRE( x == 42 );
    REQUIRE( sm.get( h0 ) != nullptr );
}

TEST_CASE( "basic insert" ) {
    slot_map< int > sm;
    auto h0 = sm.insert( 0 );
    REQUIRE( sm.size() == 1 );
    REQUIRE( sm.valid( h0 ) );
    REQUIRE( *sm.get( h0 ) == 0 );
}

TEST_CASE( "large insert and erase" ) {
    slot_map< int > sm;
    std::vector<slot_map< int >::handle> handlers;
    for(int i = 0; i < 1000; i++){
        handlers.push_back(sm.insert(i));
    }
    for(int i = 0; i < 1000; i++){
        REQUIRE( *sm.get( handlers[i] ) == i );
        sm.erase(handlers[i]);
    }
    REQUIRE(sm.empty());
    REQUIRE(sm.size() == 0);
}

TEST_CASE("total erase"){
    slot_map< int > sm;
    std::vector<slot_map< int >::handle> handlers;
    for(int i = 0; i < 10; i++){
        handlers.push_back(sm.insert(i));
    }
    for(int i = 0; i < 10; i++){
        REQUIRE( *sm.get( handlers[i] ) == i );
        REQUIRE(sm.size() == 10 - i);
        sm.erase(handlers[i]);
    }
    REQUIRE(sm.empty());
    //nothing happens
    sm.erase(handlers[1]);
    sm.insert(0);
    REQUIRE(!sm.empty());


}

TEST_CASE("copy + asign"){
    slot_map< int > sm;
    std::vector<slot_map< int >::handle> handlers;
    for(int i = 0; i < 10; i++){
        handlers.push_back(sm.insert(i));
    }
    slot_map< int > copy(sm);
    slot_map< int > assigned;
    assigned = sm;
    REQUIRE(sm == copy);
    REQUIRE(sm == assigned);

}

TEST_CASE("swap"){
    slot_map< int > fst;
    std::vector<slot_map< int >::handle> handlers;
    for(int i = 0; i < 10; i++){
        fst.insert(i);
    }
    slot_map< int > snd;
    REQUIRE(fst.size() == 10);
    REQUIRE(!fst.empty());
    REQUIRE(snd.size() == 0);
    REQUIRE(snd.empty());
    fst.swap(snd);
    REQUIRE(snd.size() == 10);
    REQUIRE(!snd.empty());
    REQUIRE(fst.size() == 0);
    REQUIRE(fst.empty());
    swap(fst,snd);
    REQUIRE(fst.size() == 10);
    REQUIRE(!fst.empty());
    REQUIRE(snd.size() == 0);
    REQUIRE(snd.empty());

}

TEST_CASE("const slotmap + copy"){
    slot_map< int > sm;
    std::vector<slot_map< int >::handle> handlers;
    for(int i = 0; i < 10; i++){
        handlers.push_back(sm.insert(i));
    }
    const slot_map<int> cpy = sm;
    for(int i = 0; i < 10; i++){
        REQUIRE( *cpy.get( handlers[i] ) == i );
    }

}

TEST_CASE("insert-erase-insert"){
    slot_map< int > sm;
    auto h0 = sm.insert(0);
    auto h5 = sm.insert(5);
    auto h6 = sm.insert(6);
    auto h3 = sm.insert(3);
    auto h1 = sm.insert(1);
    sm.erase(h6);
    REQUIRE(sm.get(h6) == nullptr);
    REQUIRE(sm.size() == 4);

    REQUIRE(!sm.valid(h6));
    REQUIRE(sm.valid(h5));
    REQUIRE(sm.valid(h3));
    REQUIRE(sm.valid(h0));
    REQUIRE(sm.valid(h1));

    h6 = sm.insert(6);

    REQUIRE(sm.valid(h6));
    REQUIRE(sm.valid(h5));
    REQUIRE(sm.valid(h3));
    REQUIRE(sm.valid(h0));
    REQUIRE(sm.valid(h1));
}

TEST_CASE("handle interface"){
    slot_map<int>::handle h;
    slot_map<int>::handle cpy(h);
    slot_map<int>::handle assigned;
    assigned = h;
    const slot_map<int>::handle c_h;
    const slot_map<int>::handle c_cpy(h);
    const slot_map<int>::handle c_assigned;
    assigned = h;
}

TEST_CASE("Big objects"){
    slot_map< std::vector<int> > sm;
    std::vector<slot_map< std::vector<int> >::handle> handlers;
    for(int i = 1; i < 10; i++){
        handlers.push_back(sm.insert(std::vector<int>(i, 0)));
    }
    for(int i = 0; i < 9; i++){
        std::vector<int> actual = *sm.get( handlers[i] );
        REQUIRE( actual.size()  == i+1 );
        REQUIRE( actual[i] == 0);
        sm.erase(handlers[i]);
    }
    REQUIRE(sm.empty());
    //nothing happens
    sm.erase(handlers[1]);
    sm.insert({0});
    REQUIRE(!sm.empty());
}

TEST_CASE("long usage"){
    slot_map< int > sm;
    auto h0 = sm.insert(0);
    auto h1 = sm.insert(1);
    auto h4 = sm.insert(4);
    auto h2 = sm.insert(2);
    auto h5 = sm.insert(5);
    auto h6 = sm.insert(6);
    auto h3 = sm.insert(3);
    auto h7 = sm.insert(7);
    sm.erase(h2);
    sm.erase(h4);
    sm.erase(h6);
    REQUIRE(sm[h0] == 0);
    REQUIRE(sm[h1] == 1);
    REQUIRE(sm[h3] == 3);
    REQUIRE(sm[h5] == 5);
    REQUIRE(sm[h7] == 7);
    auto h8 = sm.insert(8);
    auto h9 = sm.insert(9);
    sm.erase(h1);
    sm.erase(h8);
    REQUIRE(sm[h0] == 0);
    REQUIRE(sm[h3] == 3);
    REQUIRE(sm[h5] == 5);
    REQUIRE(sm[h7] == 7);
    REQUIRE(sm[h9] == 9);

    REQUIRE( sm.valid(h0));
    REQUIRE(!sm.valid(h1));
    REQUIRE(!sm.valid(h2));
    REQUIRE( sm.valid(h3));
    REQUIRE(!sm.valid(h4));
    REQUIRE( sm.valid(h5));
    REQUIRE(!sm.valid(h6));
    REQUIRE( sm.valid(h7));
    REQUIRE(!sm.valid(h8));
    REQUIRE( sm.valid(h9));

    REQUIRE(sm.size() == 5);

}


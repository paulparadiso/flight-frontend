//
//  WeatherParticulate.cpp
//  flight
//
//  Created by Paul Paradiso on 3/15/16.
//
//

#include "WeatherParticulate.h"

WeatherParticulate::WeatherParticulate(){
    
}

void WeatherParticulate::update(string _data){
    ofxJSONElement data;
    data.parse(_data);
}
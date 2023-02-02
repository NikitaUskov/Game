#include "Unit.h"

namespace RPG {

Unit::Unit(std::string name, UNIT_TYPE type0, coordinate position):
                                                                                    name(std::move(name)),
                                                                                    type(type0),
                                                                                    position(std::move(position)),
                                                                                    params()
                                                                                    {}


coordinate Unit::get_position() const noexcept {
    return position;
}

UNIT_TYPE Unit::get_type() {
    return type;
}

basic_unit_parameters Unit::get_params() const noexcept{
    return params;
}

//int Unit::change_time(int loss_time) {
//
//    if (params.current_time < loss_time) {
//        return -1;
//    }
//    params.current_time -= loss_time;
//    return params.current_time;
//}

int Unit::get_damage(int loss) {
    params.current_health -= loss;
    if (params.current_health <= 0) {
        params.current_health = 0;
        return 100;
    }
    return 0;
}

int Unit::step(Direction new_position, Map& map_) {

    if (params.current_time <= 0) {
        return 1; // haven't time
    }
    coordinate prob_pos;
    prob_pos.first = position.first + DIR_POS[new_position].first;
    prob_pos.second = position.second + DIR_POS[new_position].second;
    if (!(map_[prob_pos]->can_go_through())) {
        return -1;
    }
    position.first = prob_pos.first;
    position.second = prob_pos.second;
    params.current_time -= params.time_to_step;
    return 0;
}

void Unit::update_time() {
    params.current_time = params.max_time;
}

void Unit::setParams(int health, int time) {
    params.current_health = health;
    params.current_time = time;
}

} // RPG

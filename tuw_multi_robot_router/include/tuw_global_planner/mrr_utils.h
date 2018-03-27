#ifndef POINT_H
#define POINT_H

#include <eigen3/Eigen/Dense>
#include <tuw_global_planner/srr_utils.h>

namespace multi_robot_router
{
    struct PotentialPoint
    {
        Eigen::Vector2d point;
        float potential;
        PotentialPoint(float _x, float _y, float _potential) : point(_x, _y), potential(_potential) {};
        PotentialPoint(): PotentialPoint(0, 0, 0) {};
    } ;


    class Checkpoint
    {
        public:
            struct Precondition
            {
                int32_t robotId;
                int32_t stepCondition;
            };

            uint32_t segId;
            Eigen::Vector2d start;
            Eigen::Vector2d end;
            std::vector<Precondition> preconditions;

            Checkpoint(const RouteVertex &_v)
            {
                segId = _v.getSegment().getSegmentId();

                if(_v.direction == RouteVertex::path_direction::start_to_end)
                {
                    start = _v.getSegment().getStart();
                    end = _v.getSegment().getEnd();
                }
                else
                {
                    start = _v.getSegment().getEnd();
                    end = _v.getSegment().getStart();
                }
            }
            Checkpoint(): segId(-1)
            { }

            void updatePreconditions(const Precondition &n_pc)
            {
                bool updatedPc = false;

                for(Checkpoint::Precondition & pc : preconditions)
                {
                    if(pc.robotId == n_pc.robotId)
                    {
                        pc.stepCondition = std::max(pc.stepCondition, n_pc.stepCondition);
                        updatedPc = true;
                        break;
                    }
                }

                if(!updatedPc)
                {
                    preconditions.push_back(n_pc);
                }
            }
            
            
            void updatePreconditions(const std::vector<Precondition> &n_pcs)
            {
                for(const Checkpoint::Precondition &n_pc : n_pcs)
                {
                    bool updatedPc = false;

                    for(Checkpoint::Precondition & pc : preconditions)
                    {
                        if(pc.robotId == n_pc.robotId)
                        {
                            pc.stepCondition = std::max(pc.stepCondition, n_pc.stepCondition);
                            updatedPc = true;
                            break;
                        }
                    }

                    if(!updatedPc)
                    {
                        preconditions.push_back(n_pc);
                    }      
                }
            }
    };
}
#endif
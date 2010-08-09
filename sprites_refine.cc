#include "sprites.hh"

void SpriteLore::FindDistinctActors()
{
    const unsigned nframes = DifferencesEachFrame.size();
    const unsigned NonActorId = ~unsigned(0);

    /* List of actor numbers corresponding to each original bounding box */
    VecType< VecType< unsigned > > ActorMap(nframes);

    for(unsigned frameno=0; frameno<nframes; ++frameno)
    {
        const BoundingBoxListType& boxes        = DifferencesEachFrame[frameno];
        VecType<unsigned>        & boxes_actors = ActorMap[frameno];
        boxes_actors.resize( boxes.size() );

        if(frameno == 0)
        {
            // First frame: All bounding boxes constitute a new actor, each
            for(unsigned b=0; b<boxes.size(); ++b)
            {
                const BoundingBox& cur_box       = boxes[b];
                unsigned&          cur_box_actor = boxes_actors[b];

                unsigned actor_id = Actors.size();
                Actors[actor_id][frameno] = cur_box;
                cur_box_actor = actor_id;
            }
            continue;
        }

        MapType<unsigned/*actorid*/, VecType<unsigned/*boxid*/> > actor_matches;
        VecType</*box, */            VecType<unsigned/*actorid*/> > box_matches(boxes.size());

        for(unsigned b=0; b<boxes.size(); ++b)
        {
            // Compare to the previous frame to see where the actors moved
            const BoundingBox& cur_box = boxes[b];

            const BoundingBoxListType& prev_boxes        = DifferencesEachFrame[frameno-1];
            VecType<unsigned>        & prev_boxes_actors = ActorMap[frameno-1];

            /* Make an initial estimate: All boxes belong
             * to the same actor as they did on the previous frame.
             */
            for(unsigned p=0; p<prev_boxes.size(); ++p)
            {
                const BoundingBox& prev_box       = prev_boxes[p];
                const unsigned     prev_box_actor = prev_boxes_actors[p];
                if(cur_box.Overlaps(prev_box)
                && prev_box_actor != NonActorId)
                {
                    box_matches[b].push_back(prev_box_actor);
                    actor_matches[prev_box_actor].push_back(b);
                }
            }
        }

        for(unsigned b=0; b<boxes.size(); ++b)
        {
            const BoundingBox& cur_box       = boxes[b];
            unsigned&          cur_box_actor = boxes_actors[b];

            if(box_matches[b].empty())
            {
                // There was nothing in the previous frame that overlapped
                // with this bounding box. Guess this is a new actor.
                unsigned actor_id = Actors.size();
                Actors[actor_id][frameno] = cur_box;
                cur_box_actor = actor_id;
            }
            else if(box_matches[b].size() == 1)
            {
                // The box overlapped with a single previous-frame actor.
                // Verify that the actor did not overlap with other boxes too.
                const unsigned actor_id = box_matches[b].front();
                if(actor_matches[actor_id].size() == 1
                && actor_id != NonActorId)
                {
                    // SINGLE ACTOR ==> SINGLE ACTOR
                    // Clean match
                    Actors[actor_id][frameno] = cur_box;
                    cur_box_actor = actor_id;
                }
                else
                {
                    // SINGLE ACTOR ==> MULTIPLE ACTORS (projectile spawn)
                    // The actor hit this box and also some other boxes.
                    // Assign no actor to this box.
                    cur_box_actor = NonActorId;
                }
            }
            else
            {
                // The box overlapped with multiple previous-frame actors.
                // MULTIPLE ACTORS ==> SINGLE ACTOR  (projectile absorb)
                // MULTIPLE ACTORS ==> MULTIPLE ACTORS
                // Assign no actor to this box.
                cur_box_actor = NonActorId;
            }
        } // for b in boxes
    } // for f in frames
}

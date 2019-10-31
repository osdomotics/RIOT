/*
 * Copyright (C) 2019 Marcus Priesch <marcus@priesch.co.at>
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     net_gnrc_rpl
 * @{
 * @file
 * @brief       Objective Function MRHOF.
 *
 * Implementation of Objective Function MRHOF.
 *
 * @author      Eric Engel <eric.engel@fu-berlin.de>
 *              Ralf Schlatterbeck <rsc@runtux.com>
 *              Marcus Priesch <marcus@priesch.co.at>
 * @}
 */

#include <string.h>
#include "of_mrhof.h"
#include "net/gnrc/rpl.h"
#include "net/gnrc/rpl/structs.h"
#include "gnrc_rpl_internal/globals.h"

//#include "etx_beaconing.h"

#define ENABLE_DEBUG    (1)
#include "debug.h"

static uint16_t calc_rank(gnrc_rpl_dodag_t *, uint16_t);
static int parent_cmp(gnrc_rpl_parent_t *, gnrc_rpl_parent_t *);
static gnrc_rpl_dodag_t *which_dodag(gnrc_rpl_dodag_t *, gnrc_rpl_dodag_t *);
static void reset(gnrc_rpl_dodag_t *);

static uint16_t calc_path_cost(gnrc_rpl_parent_t *parent);

static gnrc_rpl_of_t gnrc_rpl_of_mrhof = {
    .ocp          = 0x1,
    .calc_rank    = calc_rank,
    .parent_cmp   = parent_cmp,
    .which_dodag  = which_dodag,
    .reset        = reset,
    .parent_state_callback = NULL,
    .init         = NULL,
    .process_dio  = NULL
};

gnrc_rpl_of_t *gnrc_rpl_get_of_mrhof(void)
{
    return &gnrc_rpl_of_mrhof;
}

void reset(gnrc_rpl_dodag_t *dodag)
{
    (void) dodag;
}

static uint16_t calc_path_cost(gnrc_rpl_parent_t *parent)
{
    DEBUG("calc_pathcost\n");

    /*
     * Calculates the path cost through the parent, for now, only for ETX
     */
    if (parent == NULL) {
        // Shouldn't ever happen since this function is supposed to be always
        // run with a parent. If it does happen, we can assume a root called it.
        DEBUG("[WARNING] calc_path_cost called without parent!\n");
        return GNRC_RPL_DEFAULT_MIN_HOP_RANK_INCREASE;
    }

    double etx_value = 1;//etx_get_metric(&(parent->addr));
    //DEBUG("Metric for parent returned: %f\n", etx_value);

    if (etx_value != 0) {
        /*
         * (ETX_for_link_to_neighbor * 128) + Rank_of_that_neighbor
         *
         * This means I get the rank of that neighbor (which is the etx
         * of the whole path from him to the root node) plus my ETX to
         * that neighbor*128, which would be the 'rank' of the single link
         * from me to that neighbor
         *
         */
        if (etx_value * ETX_RANK_MULTIPLIER > MAX_LINK_METRIC) {
            // Disallow links with an estimated ETX of 4 or higher
            return MAX_PATH_COST;
        }

        if (etx_value * ETX_RANK_MULTIPLIER + parent->rank
            < parent->rank) {
            //Overflow
            return MAX_PATH_COST;
        }

        //TODO runden
        return etx_value * ETX_RANK_MULTIPLIER
               + parent->rank;
    }
    else {
        // IMPLEMENT HANDLING OF OTHER METRICS HERE
        // if it is 0, it hasn't been computed, thus we cannot compute a path
        // cost
        return MAX_PATH_COST;
    }
}

static uint16_t calc_rank(gnrc_rpl_dodag_t *dodag, uint16_t base_rank)
{
    DEBUG("calc_rank\n");

    /*
     * Return the rank for this node.
     *
     * For now, there is no metric-selection or specification, so the rank com-
     * putation will always be assumed to be done for the ETX metric.
     * Baserank is pretty much only used to find out if a node is a root or not.
     */
    if (dodag->parents == NULL) {
        if (base_rank == 0) {
            //No parent, no rank, a root node would have a rank != 0
            return GNRC_RPL_INFINITE_RANK;
        }

        /*
         * No parent, base_rank != 0 means this is a root node or a node which
         * is recalculating.
         * Since a recalculating node must have a parent in this implementation
         * (see rpl.c, function global_repair), we can assume this node is root.
         */
        return GNRC_RPL_DEFAULT_MIN_HOP_RANK_INCREASE;
    }
    else {
        /*
         * We have a parent and are a non-root node, calculate the path cost for
         * the parent and choose the maximum of that value and the advertised
         * rank of the parent + minhoprankincrease for our rank.
         */
        uint16_t calculated_pcost = calc_path_cost(dodag->parents);

        if (calculated_pcost < MAX_PATH_COST) {
            if ((dodag->parents->rank + GNRC_RPL_DEFAULT_MIN_HOP_RANK_INCREASE)
                > calculated_pcost) {
                return dodag->parents->rank + GNRC_RPL_DEFAULT_MIN_HOP_RANK_INCREASE;
            }
            else {
                return calculated_pcost;
            }
        }
        else {
            //Path costs are greater than allowed
            return GNRC_RPL_INFINITE_RANK;
        }
    }
}

static int parent_cmp(gnrc_rpl_parent_t *parent1, gnrc_rpl_parent_t *parent2)
{
    DEBUG("which_parent\n");
    /*
     * Return the parent with the lowest path cost.
     * Before returning any of the two given parents, make sure that a switch is
     * desirable.
     *
     */
    uint16_t path_p1    = calc_path_cost(parent1);
    uint16_t path_p2    = calc_path_cost(parent2);

    if (parent1->state == GNRC_RPL_PARENT_ACTIVE) {
        path_p2 += PARENT_SWITCH_THRESHOLD;
    } else if (parent2->state == GNRC_RPL_PARENT_ACTIVE) {
        path_p1 += PARENT_SWITCH_THRESHOLD;
    }

    if (path_p1 < path_p2) {
        return -1;
    } else if (path_p1 > path_p2) {
        return 1;
    }
    return 0;
}

//Not used yet, as the implementation only makes use of one dodag for now.
static gnrc_rpl_dodag_t *which_dodag(gnrc_rpl_dodag_t *d1, gnrc_rpl_dodag_t *d2)
{
    (void) d2;
    return d1;
}

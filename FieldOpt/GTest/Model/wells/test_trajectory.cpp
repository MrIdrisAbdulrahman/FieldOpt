/******************************************************************************
 *
 *
 *
 * Created: 04.11.2015 2015 by einar
 *
 * This file is part of the FieldOpt project.
 *
 * Copyright (C) 2015-2015 Einar J.M. Baumann <einar.baumann@ntnu.no>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA
 *****************************************************************************/

#include <gtest/gtest.h>
#include "Model/wells/wellbore/trajectory.h"
#include "Model/wells/well_exceptions.h"

using namespace Model::Wells;

namespace {

class TrajectoryTest : public ::testing::Test {

protected:
    TrajectoryTest() {
        settings_ = new ::Utilities::Settings::Settings(driver_file_path_);
        variable_handler_ = new ::Model::Variables::VariableHandler(*settings_->model());
        variable_container_ = new ::Model::Variables::VariableContainer();
        prod_well_settings_ = settings_->model()->wells().first();
        prod_well_trajectory_ = new Wellbore::Trajectory(prod_well_settings_, variable_container_, variable_handler_);
    }
    virtual ~TrajectoryTest() {}
    virtual void SetUp() {}

    QString driver_file_path_ = "../../FieldOpt/GTest/Utilities/driver/driver.json";
    ::Utilities::Settings::Settings *settings_;
    ::Utilities::Settings::Model::Well prod_well_settings_;
    ::Model::Variables::VariableHandler *variable_handler_;
    ::Model::Variables::VariableContainer *variable_container_;
    ::Model::Wells::Wellbore::Trajectory *prod_well_trajectory_;
};

TEST_F(TrajectoryTest, Constructor) {
    EXPECT_TRUE(true);
}

TEST_F(TrajectoryTest, GetWellBlock) {
    EXPECT_NO_THROW(prod_well_trajectory_->GetWellBlock(0, 4, 2));
    EXPECT_THROW(prod_well_trajectory_->GetWellBlock(9, 9, 9), WellBlockNotFoundException);

    EXPECT_EQ(0, prod_well_trajectory_->GetWellBlock(0,4,2)->i());
    EXPECT_EQ(4, prod_well_trajectory_->GetWellBlock(0,4,2)->j());
    EXPECT_EQ(2, prod_well_trajectory_->GetWellBlock(0,4,2)->k());
    EXPECT_FALSE(prod_well_trajectory_->GetWellBlock(0,4,2)->HasCompletion());

    EXPECT_EQ(1, prod_well_trajectory_->GetWellBlock(1,4,2)->i());
    EXPECT_EQ(4, prod_well_trajectory_->GetWellBlock(1,4,2)->j());
    EXPECT_EQ(2, prod_well_trajectory_->GetWellBlock(1,4,2)->k());
    EXPECT_TRUE(prod_well_trajectory_->GetWellBlock(1,4,2)->HasCompletion());

    EXPECT_EQ(2, prod_well_trajectory_->GetWellBlock(2,4,2)->i());
    EXPECT_EQ(4, prod_well_trajectory_->GetWellBlock(2,4,2)->j());
    EXPECT_EQ(2, prod_well_trajectory_->GetWellBlock(2,4,2)->k());
    EXPECT_TRUE(prod_well_trajectory_->GetWellBlock(2,4,2)->HasCompletion());

    EXPECT_EQ(3, prod_well_trajectory_->GetWellBlock(3,4,2)->i());
    EXPECT_EQ(4, prod_well_trajectory_->GetWellBlock(3,4,2)->j());
    EXPECT_EQ(2, prod_well_trajectory_->GetWellBlock(3,4,2)->k());
    EXPECT_FALSE(prod_well_trajectory_->GetWellBlock(3,4,2)->HasCompletion());
}

TEST_F(TrajectoryTest, AllWellBlocks) {
    EXPECT_EQ(4, prod_well_trajectory_->GetWellBlocks()->size());
    EXPECT_FALSE(prod_well_trajectory_->GetWellBlocks()->at(0)->HasCompletion());
    EXPECT_TRUE(prod_well_trajectory_->GetWellBlocks()->at(1)->HasCompletion());
    EXPECT_TRUE(prod_well_trajectory_->GetWellBlocks()->at(2)->HasCompletion());
    EXPECT_FALSE(prod_well_trajectory_->GetWellBlocks()->at(3)->HasCompletion());
}

TEST_F(TrajectoryTest, Completions) {
    EXPECT_EQ(::Model::Wells::Wellbore::Completions::Completion::CompletionType::Perforation,
              prod_well_trajectory_->GetWellBlock(1,4,2)->GetCompletion()->type());
    EXPECT_FALSE(prod_well_trajectory_->GetWellBlock(0,4,2)->HasPerforation());
    EXPECT_TRUE(prod_well_trajectory_->GetWellBlock(1,4,2)->HasPerforation());
    EXPECT_TRUE(prod_well_trajectory_->GetWellBlock(2,4,2)->HasPerforation());
    EXPECT_FALSE(prod_well_trajectory_->GetWellBlock(3,4,2)->HasPerforation());
    EXPECT_FLOAT_EQ(1.0, prod_well_trajectory_->GetWellBlock(1,4,2)->GetPerforation()->transmissibility_factor());
    EXPECT_FLOAT_EQ(1.0, prod_well_trajectory_->GetWellBlock(2,4,2)->GetPerforation()->transmissibility_factor());

    EXPECT_THROW(prod_well_trajectory_->GetWellBlock(0,4,2)->GetPerforation(), PerforationNotDefinedForWellBlockException);
}

TEST_F(TrajectoryTest, VariableHandlerCorrectness) {
    EXPECT_TRUE(variable_handler_->GetWellBlock(0)->position());
    EXPECT_TRUE(variable_handler_->GetWellBlock(1)->position());
    EXPECT_TRUE(variable_handler_->GetWellBlock(2)->position());
    EXPECT_TRUE(variable_handler_->GetWellBlock(3)->position());
}


TEST_F(TrajectoryTest, VariableContainerConsistencyAfterCreation) {
    // There should be three integer variables (i,j,k) for each of the four well block
    EXPECT_EQ(12, variable_container_->IntegerVariableSize());
    EXPECT_EQ(0, variable_container_->GetIntegerVariable(0)->value()); // Block 1, i
    EXPECT_EQ(4, variable_container_->GetIntegerVariable(1)->value()); // Block 1, j
    EXPECT_EQ(2, variable_container_->GetIntegerVariable(2)->value()); // Block 1, k
    EXPECT_EQ(3, variable_container_->GetIntegerVariable(9)->value()); // Block 4, i
    EXPECT_EQ(4, variable_container_->GetIntegerVariable(10)->value()); // Block 4, j
    EXPECT_EQ(2, variable_container_->GetIntegerVariable(11)->value()); // Block 4, k
}

TEST_F(TrajectoryTest, VariableContainerConsistencyAfterModification) {
    // Change values directly and verify change in variable container
    prod_well_trajectory_->GetWellBlocks()->at(0)->setI(5);
    prod_well_trajectory_->GetWellBlocks()->at(0)->setJ(6);
    prod_well_trajectory_->GetWellBlocks()->at(0)->setK(7);
    EXPECT_EQ(5, variable_container_->GetIntegerVariable(0)->value()); // Block 1, i
    EXPECT_EQ(6, variable_container_->GetIntegerVariable(1)->value()); // Block 1, j
    EXPECT_EQ(7, variable_container_->GetIntegerVariable(2)->value()); // Block 1, k

    // Change value in variable container and verify change directly
    variable_container_->GetIntegerVariable(0)->setValue(7);
    variable_container_->GetIntegerVariable(1)->setValue(8);
    variable_container_->GetIntegerVariable(2)->setValue(9);
    EXPECT_EQ(7, prod_well_trajectory_->GetWellBlocks()->at(0)->i());
    EXPECT_EQ(8, prod_well_trajectory_->GetWellBlocks()->at(0)->j());
    EXPECT_EQ(9, prod_well_trajectory_->GetWellBlocks()->at(0)->k());
}

TEST_F(TrajectoryTest, WellBlockPerforationConsistency) {
    // The first well block should not have a completion
    EXPECT_FALSE(prod_well_trajectory_->GetWellBlock(0)->HasCompletion());

    // The first completion should reside in the second well block
    EXPECT_EQ(0, prod_well_trajectory_->GetWellBlock(1)->GetPerforation()->id());

    // The second completion should reside in the third well block
    EXPECT_EQ(1, prod_well_trajectory_->GetWellBlock(2)->GetPerforation()->id());

    // The fourth well block should not have a completion
    EXPECT_FALSE(prod_well_trajectory_->GetWellBlock(3)->HasCompletion());
}


}


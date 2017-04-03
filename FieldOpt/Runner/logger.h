/******************************************************************************
   Copyright (C) 2015-2017 Einar J.M. Baumann <einar.baumann@gmail.com>

   This file is part of the FieldOpt project.

   FieldOpt is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   FieldOpt is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with FieldOpt.  If not, see <http://www.gnu.org/licenses/>.
******************************************************************************/

#ifndef LOGGER_H
#define LOGGER_H

#include "string"
#include "map"
#include <QString>
#include <QStringList>
#include <QDateTime>
#include <QUuid>
#include "Optimization/case.h"
#include "Optimization/optimizer.h"
#include "runtime_settings.h"
#include "Settings/settings.h"
#include "Model/model.h"
#include "Simulation/results/results.h"
#include "loggable.hpp"

using namespace std;

namespace Runner {

/*!
 * \brief The Logger class is responsible for writing CSV logs to the disk, as well as printing
 * information to the console at runtime.
 */
class Logger
{
 public:
  /*!
   * \brief Logger
   * \param rts Runtime settings
   * \param output_subdir Optional subdirectory in the output directory to write the logs in.
   */
  Logger(RuntimeSettings *rts, QString output_subdir="");

  void AddEntry(Loggable &obj);


 private:
  bool verbose_; //!< Whether or not new log entries should also be printed to the console.
  QString output_dir_; //!< Directory in which the files will be written.
  QString opt_log_path_; //!< Path to the optimization log file.
  QString cas_log_path_; //!< Path to the case log file.

  /*!
   * @brief The column widths count from after the leading comma (if there is one) up to the
   * last letter in the name of the column. The trailing space and comma are added separately.
   */
  const map<string, int> cas_log_col_widths_ = {{"TimeSt", 19 },
                                                {"EvalSt", 7}, {"ConsSt", 7}, {"ErrMsg", 7},
                                                {"SimDur", 9},
                                                {"OFnVal", 12},
                                                {"CaseId", 41}
  };
  const QString cas_log_header_ = "             TimeSt , EvalSt , ConsSt , ErrMsg ,   SimDur ,      OFnVal ,                                 CaseId";
  const map<string, int> opt_log_col_widths_ = {{"TimeSt", 19},
                                                {"TimeEl", 9},
                                                {"IterNr", 7}, {"TotlNr", 7}, {"BkpdNr", 7}, {"TimONr", 7},
                                                {"FailNr", 7}, {"InvlNr", 7}, {"IterNr", 7},
                                                {"CBOFnV", 12},
                                                {"CurBst", 41}
  };
  const QString opt_log_header_ = "             TimeSt ,   TimeEl , IterNr , TotlNr , EvalNr , BkpdNr , TimONr , FailNr , InvlNr ,      CBOFnV ,                                 CurBst";

  void logCase(Loggable &obj);
  void logOptimizer(Loggable &obj);
  void logExtended(Loggable &obj);
};

}

#endif // LOGGER_H

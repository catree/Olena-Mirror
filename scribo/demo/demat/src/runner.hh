// Copyright (C) 2010 EPITA Research and Development Laboratory (LRDE)
//
// This file is part of Olena.
//
// Olena is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, version 2 of the License.
//
// Olena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Olena.  If not, see <http://www.gnu.org/licenses/>.
//
// As a special exception, you may use this file as part of a free
// software project without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to produce
// an executable, this file does not by itself cause the resulting
// executable to be covered by the GNU General Public License.  This
// exception does not however invalidate any other reasons why the
// executable file might be covered by the GNU General Public License.

#ifndef SCRIBO_DEMO_SHARED_SRC_RUNNER_HH
# define SCRIBO_DEMO_SHARED_SRC_RUNNER_HH

# include <QtGui>
# include <QProcess>

# include <src/preprocessing_task.hh>
# include <src/doc_type.hh>

# include <mln/core/image/image2d.hh>
# include <mln/value/qt/rgb32.hh>

#include <src/process_args.hh>

namespace scribo
{

  namespace demo
  {


    class runner : public QThread
    {
      Q_OBJECT;

    public:
      runner(QObject *parent = 0);

      void start(const QString& filename,
		 const mln::image2d<mln::value::rgb8>& input,
		 const Doc_Type& doc_type,
		 const preprocessing_task_set_t& tasks,
		 const process_args& args);

      void stop();

    signals:
      void new_intermediate_result(const QImage& ima);
      void new_progress_max_value(unsigned i);
      void new_progress_label(const QString& msg);
      void progress(unsigned i);
      void finished();

    private: // members
      mln::image2d<mln::value::rgb8> load();
      void preprocess(const mln::image2d<mln::value::rgb8>& ima);
      void process();
      virtual void run();

      template <typename V>
      unsigned find_best_scale(const mln::image2d<V>& ima);

    private: // attributes
      QProcess process_;
      Doc_Type doc_type_;
      QSet<Preprocessing_Task> tasks_;
      QString filename_;
      process_args args_;
      mln::image2d<mln::value::rgb8> input_;
    };



  } // end of namespace scribo::demo

} // end of namespace scribo


#endif // ! SCRIBO_DEMO_SHARED_SRC_RUNNER_HH

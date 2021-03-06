# -*- Mode: python; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.
#

from uitest_helper import UITest

from helper import mkPropertyValues

from UITestCase import UITestCase

import time

try:
    import pyuno
    import uno
    import unohelper
except ImportError:
    print("pyuno not found: try to set PYTHONPATH and URE_BOOTSTRAP variables")
    print("PYTHONPATH=/installation/opt/program")
    print("URE_BOOTSTRAP=file:///installation/opt/program/fundamentalrc")
    raise

class GridWinTest(UITestCase):

    def test_select_cell(self):

        self.ui_test.create_doc_in_start_center("calc")
        xCalcDoc = self.xUITest.getTopFocusWindow()
        xGridWindow = xCalcDoc.getChild("grid_window")

        selectProps = mkPropertyValues({"CELL": "B10"})
        xGridWindow.executeAction("SELECT", selectProps)

        self.ui_test.close_doc()

    def test_select_range(self):

        self.ui_test.create_doc_in_start_center("calc")
        xCalcDoc = self.xUITest.getTopFocusWindow()
        xGridWindow = xCalcDoc.getChild("grid_window")

        selectProps = mkPropertyValues({"RANGE": "B10:C20"})
        xGridWindow.executeAction("SELECT", selectProps)

        self.ui_test.close_doc()

    def test_extend_range(self):

        self.ui_test.create_doc_in_start_center("calc")
        xTopWindow = self.xUITest.getTopFocusWindow()

        xGridWindow = xTopWindow.getChild("grid_window")

        selectProps = mkPropertyValues({"RANGE": "B10:C20"})
        xGridWindow.executeAction("SELECT", selectProps)

        select2Props = mkPropertyValues({"RANGE": "D3:F5", "EXTEND": "true"})
        xGridWindow.executeAction("SELECT", select2Props)

        self.ui_test.close_doc()

# vim:set shiftwidth=4 softtabstop=4 expandtab: */

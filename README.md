Google Tag Manager
==================

This Google Tag Manager project allows non-technical stakeholders to manage the
analytics for their website by triggering the insertion of tags and tracking
systems onto their page(s) via Google's Tag Manager (GTM) hosted application.

Installation
------------

- Install this module using the [official Backdrop CMS instructions](https://backdropcms.org/guide/modules).

- Visit the configuration page under **Administration > Configuration > System >
  Google Tag Manager** (`admin/config/system/google_tag`) and enter the required
  information.

Requirements
-------------

Sign up for GTM and obtain a 'container ID' for your website. Enter the
'container ID' on the settings form for this module (see Configuration).

 * https://www.google.com/analytics/tag-manager/

Issues
------

Bugs and Feature requests should be reported in the [Issue Queue](https://github.com/backdrop-contrib/google_tag/issues).

Troubleshooting
---------------

If the JavaScript snippets are not present in the HTML output, try the following
steps to debug the situation:

 * Confirm the snippet files exist at `public://google_tag/` (on most sites this
   equates to `files/google_tag/`).

   If missing, then visit the module settings page and submit the form to
   recreate the snippet files. The need to do this may arise if the project is
   deployed from one environment to another (e.g. development to production) but
   the snippet files are not deployed.

 * Enable debug output on the 'Advanced' tab of the settings page to display the
   result of each snippet insertion condition in the message area. Modify the
   insertion conditions as needed.

Current Maintainers
-------------------

- Jen Lampton (https://github.com/jenlampton).
- Kevin Thull (https://github.com/kthull).
- Laryn Kragt Bakker (https://github.com/laryn).
- Seeking additional maintainers.

Credits
-------

- Ported to Backdrop CMS by Kevin Thull (https://github.com/kthull).
- Originally written for Drupal by Jim Berry ("solotandem", http://drupal.org/user/240748).

License
-------

This project is GPL v2 software. See the LICENSE.txt file in this directory for
complete text.

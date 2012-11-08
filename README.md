#User_Permission
##Retrieve
* Args:
* HTTP Method : GET
* Example URL : http://andrd.yarrait.com/drupal6/android/user_permission/block?role=1
* Expected Response(in JSON):

{"modules": ["advanced_help","block","blog","bueditor","category","comment","contact","content","content_permissions","ctools_access_ruleset"]}

##Update
* Args: permission*
* HTTP Method: PUT 
* Example URL : http://andrd.yarrait.com/drupal6/android/user_permission
* Example: perm[module]=block,perm[enabled][0]=php block visiblity,perm[enabled][1]=block visiblity
* Expected Response(in JSON):
`

#Role
##Retrieve
* Args:
* HTTP Method : GET
* Example URL : http://andrd.yarrait.com/drupal6/android/role/20
* Expected Response(in JSON):
 `{"rid": "20","name": "ghjgg","uri": "http://localhost/drupal-6.26/android/role/20"},

##Create
* Args: role*
* HTTP Method: POST
* Example URL : http://andrd.yarrait.com/drupal6/android/role
* Example: name=mahipal(Name of role which add)
* Expected Response(in JSON):
   `{"rid":"24","uri":"http://andrd.yarrait.com/drupal6/android/role/24"}`

##Update
* Args: role*
* HTTP Method: PUT 
* Example URL : http://andrd.yarrait.com/drupal6/android/role/24
* Example:role[name]=mahipal(Name of role which update)
* Expected Response(in JSON):
`"1"`

##Delete
* Args: 
* HTTP Method: DELETE
* Example URL : http://andrd.yarrait.com/drupal6/android/role/24
* Example: 
* Expected Response(in JSON):
`1`

#accessrule
##Retrieve
* Args:
* HTTP Method : GET
* Example URL : http://andrd.yarrait.com/drupal6/android/accessrule/35
* Expected Response(in JSON):
 `{"aid": "35","mask": "errrettttyyeftteeyyhjhfgfuu@gmail.com","type": "mail","status": "1","uri": "http://localhost/drupal-6.26/android/access/35"}`

##Create
* Args: rule*
* HTTP Method: POST
* Example URL : http://andrd.yarrait.com/drupal6/android/accessrule
* Example: access[mask]=mahi@gmail.com,access[type]=mail,access[status]=1
* Expected Response(in JSON):
  `{"aid":"35","uri":"http://localhost/drupal-6.26/android/accessrule/35"}`

##Update
* Args: rule*
* HTTP Method: PUT 
* Example URL : http://andrd.yarrait.com/drupal6/android/accessrule/35
* Example:access[mask]=mahi@gmail.com,access[type]=mail,access[status]=1
* Expected Response(in JSON):
`"1"`

##Delete
* Args: 
* HTTP Method: DELETE
* Example URL : http://andrd.yarrait.com/drupal6/android/accessrule/35
* Example: 
* Expected Response(in JSON):
`1`

#Content Type
##Retrieve
* Args:
* HTTP Method : GET
* Example URL : http://andrd.yarrait.com/drupal6/android/content_type/page
* Expected Response(in JSON):

`{"type": "type","name": "Create Account","module": "node","description": "","help": "","has_title": "1","title_label": "Title","has_body": "1","body_label": "Body",
"min_word_count": "0","custom": "1","modified": "1","locked": "0","orig_type": "","uri": "http://localhost/drupal-6.26/android/node_type/type"},`

#vocablory Type
##Retrieve
* Args:
* HTTP Method : GET
* Example URL :http://andrd.yarrait.com/drupal6/android/vocabulary/5
* Expected Response(in JSON):

`{"vid": "5","name": "Forums","description": "","help": "","relations": "1","hierarchy": "1","multiple": "0","required": "0","tags": "0","module": "forum",
"weight": "-10","nodes": {"poll": "poll"},"uri": "http://localhost/drupal-6.26/android/vocabulary/5"}`

#vocablory Term
##Retrieve
* Args:
* HTTP Method : GET
* Example URL :http://andrd.yarrait.com/drupal6/android/vocabulary_term/5(vid)
* Expected Response(in JSON):
`{"tid": "23","vid": "8","name": "tryty","description": "tytyt","weight": "0"},`

#Content
##Retrieve
* Args:
* HTTP Method : GET
* Example URL :http://andrd.yarrait.com/drupal6/android/content/22
* Expected Response(in JSON):
`{"nid": "22","vid": "66","type": "page","language": "en","title": "Resume1","uid": "1","name": "root","status": "0","created": "1346390029","changed": "1348483081",
    "comment": "2","promote": "1","moderate": "0","sticky": "0","tnid": "0","translate": "0"},`

##Create
* Args: node*
* HTTP Method: POST
* Example URL : http://andrd.yarrait.com/drupal6/android/accessrule
* Example: node[option]=status,node[filter]=published
* Expected Response(in JSON):
{"nid":"25","vid":"69","type":"joke","language":"en","title":"","uid":"3","status":"1","created":"1347865308","changed":"1347865308","comment":"2","promote":"1",
"moderate":"0","sticky":"0","tnid":"0","translate":"0"}`


#Content Update
##Update
* Args:node*
* HTTP Method : GET
* Example URL :http://andrd.yarrait.com/drupal6/android/content_update
* Example:filter=publish(unpublish,delete),node[0][nid]=22
* Expected Response(in JSON):
'1'

#Watchdog
##Retrieve
* Args:
* HTTP Method : GET
* Example URL :http://andrd.yarrait.com/drupal6/android/watchdog/1
* Expected Response(in JSON):
`{"wid": "2","uid": "1","type": "user","message": "Session opened for %name.","variables": "a:1:{s:5:\"%name\";s:4:\"root\";}","severity": "5","link": "",
"location": "http://localhost/drupal-6.26/install.php?locale=en&profile=default","referer": "http://localhost/drupal-6.26/install.php?locale=en&profile=default",
 "hostname": "127.0.0.1","timestamp": "1345617187","name": "root"},`

#User Update
##Update
* Args:user*
* HTTP Method : GET
* Example URL :http://andrd.yarrait.com/drupal6/android/user_update
* Example:filter=active(block,delete),user[0][uid]=1,user[1][uid]=2
* Expected Response(in JSON):
`1`

#Comments
##Retrieve
* Args:
* HTTP Method : GET
* Example URL :http://andrd.yarrait.com/drupal6/android/commnets
* Expected Response(in JSON):

{total": "22","array": [{"cid": "25","pid": "0","nid": "30","uid": "1","subject": "dfgd","comment": "dfgdf","hostname": "127.0.0.1","timestamp": "1349428729",
"status": "0","format": "3","thread": "01/","name": "root","mail": "","homepage": ""},
